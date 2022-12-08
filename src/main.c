#include <stdio.h>
#include "ecat_slv.h"
#include "utypes.h"
#include <bcm2835.h>
#include "esc_hw.h"
#include "log.h"
#include "user_message.h"
#include "gberror.h"
#include "gbnetx_config.h"
#include "signal.h"
#include "std_utils.h"
#include <unistd.h>
#include "linux_shm.h"
#include "pid.h"
#include "dpm.h"
#include <string.h>

/** storage for gbc process name */
char proc_name[GBC_PROCESS_NAME_MAX_LENGTH] = {0};
struct shm_msg *shmp;
int gbc_pid = 0;

//key boolean indicating if GBC has a shared mem connection to GBNETX
bool gbc_connected = false;
uint8_t inA[SIZE_OF_GBC_PDO];
uint8_t inB[SIZE_OF_GBC_PDO];
uint8_t outA[SIZE_OF_GBC_PDO];
uint8_t outB[SIZE_OF_GBC_PDO];
int kill_rc = 0;



/* Application variables */
_Objects Obj;
_Mbuffer Mb;


void cb_application(void) {
    /* Check for watchdog expired event */
    if (ESCvar.ALevent & ESCREG_ALEVENT_WD) {
        if ((ESC_WDstatus() & 0x01) &&
            ((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0)) {
            DPRINT("DIG_process watchdog expired");
            ESC_ALstatusgotoerror((ESCsafeop | ESCerror), ALERR_WATCHDOG);
        }
    }
}

void cb_state_change(uint8_t *as, uint8_t *an) {
    if (*as == SAFEOP_TO_OP) {
        /* Enable HW watchdog event */
        ESC_ALeventmaskwrite(ESC_ALeventmaskread() | ESCREG_ALEVENT_WD);
        /* Enable SM2 sync manager event */
        ESC_ALeventmaskwrite(ESC_ALeventmaskread() | ESCREG_ALEVENT_SM2);
    }
}

void cb_get_inputs(void) {
//we have a 128byte PDO on lan9252

//    Obj.in.machine_control_word = 8;
//    Obj.in.digital_5 = 1;

Obj.in.machine_control_word=dpm_out->machine_word;
Obj.in.heartbeat=dpm_out->heartbeat;
Obj.in.gbc_controlword=dpm_out->gbc_control_word;
Obj.in.hlc_controlword=dpm_out->hlc_control_word;

Obj.in.joint_0_set_position=dpm_out->joint_set_position[0];
Obj.in.joint_1_set_position=dpm_out->joint_set_position[1];




}

void cb_set_outputs(void) {
//we have a 128byte PDO on lan9252

    printf("Status word: %u\n", Obj.out.machine_status_word);
    printf("Digital out 0:%u\n", Obj.out.digital_0);

    dpm_in->machine_word=Obj.out.machine_status_word;
    dpm_in->heartbeat=Obj.out.heartbeat;
    dpm_in->active_fault_word=Obj.out.active_fault;
    dpm_in->fault_history_word=Obj.out.fault_history;

    dpm_in->joint_actual_position[0]=Obj.out.joint_0_actual_position;
    dpm_in->joint_actual_position[1]=Obj.out.joint_1_actual_position;


}

gberror_t GPIO_init(void) {
    int rc = 0;

    UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: Initialising GPIOs");

    rc = bcm2835_init();

    if (rc != 1) {
        UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_init failed (GPIO initialisation)");

        return E_GENERAL_FAILURE;
    } else {
        UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_init successful (GPIO initialisation)");
    }

#if SOES_IRQ == 1
    UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: Enabling rising edge interrupt on SYNC0 pin");
    bcm2835_gpio_fsel(SYNC0_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_ren(SYNC0_PIN);
#endif

    return E_SUCCESS;
}


void ESC_safeoutput_override(void) {
// Set safe values for digitals

    LL_DEBUG(GBLAN9252_SOES_GEN_LOG_EN, "GBLAN9252_SOES: Safe output override in operation");
    Obj.out.digital_0 = 0;
}


gberror_t ESC_wait_for_sync0(uint8_t pin) {
    uint32_t count = 0;
    bcm2835_gpio_set_eds(pin);//clear the previous event. makes sure that only next event is detected
    while ((!bcm2835_gpio_eds(pin)) && count < MAX_TIME_WAIT_SYNC0) {
        count++;
        delayMicroseconds(1);
    }//waits until next event/tick

    if (count == MAX_TIME_WAIT_SYNC0) {
        LL_ERROR(GBLAN9252_SOES_GEN_LOG_EN, "GBLAN9252_SOES: Timeout waiting sync0");
        return E_TIMEOOUT;
    }

    return E_SUCCESS;

}


_Noreturn int main_run(void *arg) {

    static esc_cfg_t config =
            {
                    .user_arg = "rpi4,cs0",
#if SOES_IRQ == 1
                    .use_interrupt = 1,
#else
                    .use_interrupt = 0,
#endif
                    .watchdog_cnt = 10000,
                    .set_defaults_hook = NULL,
                    .pre_state_change_hook = NULL,
                    .post_state_change_hook = NULL,
                    .application_hook = NULL,
                    .safeoutput_override = ESC_safeoutput_override,
                    .pre_object_download_hook = NULL,
                    .post_object_download_hook = NULL,
                    .rxpdo_override = NULL,
                    .txpdo_override = NULL,
#if SOES_IRQ == 1
                    .esc_hw_interrupt_enable = ESC_interrupt_enable,
                    .esc_hw_interrupt_disable = ESC_interrupt_disable,
#else

                    .esc_hw_interrupt_enable = NULL,
              .esc_hw_interrupt_disable = NULL,
#endif
                    .esc_hw_eep_handler = NULL,
#if SOES_IRQ == 1
                    .esc_check_dc_handler = dc_checker,
#endif
            };

    //set limit on DC before error triggered
    Obj.ErrorSettings.SyncErrorCounterLimit = 3000;


    gberror_t grc = E_GENERAL_FAILURE;




    char *username_buf;
    username_buf = (char *) malloc(11 * sizeof(char));
    memset(username_buf, 0, 11 * sizeof(char));
    if (getlogin_r(username_buf, 10) != 0) {
        strncpy(username_buf, "<unknown>", 10);
    }
    UM_INFO(GBLAN9252_SOES_UM_EN, "GBNETX: We are running as user [%s]", username_buf);


    if (SIGNAL_TO_SEND > 31) {
        UM_FATAL(
                "GBLAN9252_SOES: We have a signal number defined (with SIGNAL_TO_SEND) to a number greater than 31. This is outside the range of normal Linux signals. We must exit");
    }
    char *str = strdup(sys_siglist[SIGNAL_TO_SEND]);
    if (str) {
        upcase(str);
        UM_INFO(GBLAN9252_SOES_UM_EN,
                "GBLAN9252_SOES: We are using Linux signal [SIG %s] (we are sending this out to GBC to advance its cycle)",
                str);
        free(str);
    } else {
        UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: Error matching the signal number [%u], to the standard Linux signals",
                 SIGNAL_TO_SEND);
    }

    strcpy(proc_name, GBC_PROCESS_NAME);
    grc = establish_shared_mem_and_signal_con(&shmp, proc_name, true, &gbc_pid, 1);
    if (grc != E_SUCCESS) {
        gbc_connected = false;
        UM_ERROR(GBLAN9252_SOES_UM_EN,
                 "GBLAN9252_SOES: Connection to shared memory & GBC process could not be established - we will continue without a connection to GBC");
    } else {
        gbc_connected = true;
        UM_INFO(GBLAN9252_SOES_UM_EN,
                "GBLAN9252_SOES: We have a connection to shared memory & GBC process >successfully< established ");
        UM_INFO(GBLAN9252_SOES_UM_EN,
                "GBLAN9252_SOES: Shared memory address [%p] (this is a virtual address so will not match across processes)",
                shmp);
        memset(shmp->sm_buf_in, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
        memset(shmp->sm_buf_out, 0, sizeof(uint8_t) * SHM_BUF_SIZE);
    }


    if (GPIO_init()!=E_SUCCESS){
        UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: GPIO_init sucessful");

    }

    ecat_slv_init(&config);


    bool connected = false;





    while (1) {
#if SOES_IRQ == 1
        grc = ESC_wait_for_sync0(SYNC0_PIN);
        if (grc==E_SUCCESS) {
            LL_TRACE(GBLAN9252_SOES_GEN_LOG_EN, "Sync0 received");
        }

        /* Subtract the sync counter to check the pace compared to the SM IRQ */
        if ((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0) {
            CC_ATOMIC_SUB(ESCvar.synccounter, 1);
        }
        /* Check so we're inside the limit */
        if ((CC_ATOMIC_GET(ESCvar.synccounter) < -ESCvar.synccounterlimit) ||
            (CC_ATOMIC_GET(ESCvar.synccounter) > ESCvar.synccounterlimit)) {
            if ((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0) {
                LL_DEBUG(GBLAN9252_SOES_GEN_LOG_EN, "GBLAN9252_SOES: Sync error [%d]", ESCvar.synccounter);
                ESC_ALstatusgotoerror((ESCsafeop | ESCerror), ALERR_SYNCERROR);
                CC_ATOMIC_SET(ESCvar.synccounter, 0);
            }
        }
        ecat_slv();
        uint16_t wd_status = 0;
        ESC_read(ESCREG_WDSTATUS, &wd_status, sizeof(wd_status));

        printf("wd [0x%x]\n");
        if ((wd_status & 0x01) == 0x01) {
            printf("watchdog is inactive or disabled\n");
        } else {
            printf("Watchdog is expired\n");
        }


#else
        printf("SM2_sml:%u\n", ESCvar.ESC_SM2_sml);
        printf("SM3_sml:%u\n", ESCvar.ESC_SM3_sml);
        ecat_slv();
#endif
    }

}

int main(void) {

    //solves missing output in debugger log output (a gdb thing)
    setbuf(stdout, 0);

    /* These set where the user message output will be sent. Uncomment the one you want.
     * This is just for user messages UM_* not logging LL_ which, if enabled, always goes to stdout
    */
    logger_set_stdout();
    //    logger_set_log_file("gbem.log", GBLAN9252_SOES_UM_EN);
    //    logger_set_syslog("Glowbuzzer");



    UM_INFO(GBLAN9252_SOES_UM_EN,
            "GBLAN9252_SOES: **************************************************************************");
    UM_INFO(GBLAN9252_SOES_UM_EN,
            "GBLAN9252_SOES: ***                    Starting GBLAN9252_SOES                         ***");
    UM_INFO(GBLAN9252_SOES_UM_EN,
            "GBLAN9252_SOES: **************************************************************************");

    main_run(NULL);
    return 0;

}

///**
// * SOES main function. Called to handle watchdog
// */
//void soes_irq (void)
//{
//    DIG_process(DIG_PROCESS_WD);
//
//}
//
//void soes_isr (void)
//{
//
//    do
//    {
//        /* Check the state machine */
//        ESC_state();
//        /* Check the SM activation event */
//        ESC_sm_act_event();
//
//        /* Check mailboxes */
//        /* Check mailboxes */
//        while ((ESC_mbxprocess() > 0) || (ESCvar.txcue > 0))
//        {
//            ESC_coeprocess();
//            ESC_foeprocess();
//            ESC_xoeprocess();
//        }
//
//#if EEP_EMULATION
//        EEP_process ();
//      EEP_hw_process();
//#endif
//        ATOMIC_SET(ESCvar.ALevent,ESC_ALeventread());
//
//    }while(ESCvar.ALevent & (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                             | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP));
//
//    ESC_ALeventmaskwrite(ESC_ALeventmaskread()
//                         | (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                            | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1
//                            | ESCREG_ALEVENT_EEP));
//
//    /* On init restore PDO mappings to default size */
//    if((ESCvar.ALstatus & 0x0f) == ESCinit)
//    {
//        txpdomap = DEFAULTTXPDOMAP;
//        rxpdomap = DEFAULTRXPDOMAP;
//        txpdoitems = DEFAULTTXPDOITEMS;
//        rxpdoitems = DEFAULTTXPDOITEMS;
//        initDefaultOutputSyncMgrPara();
//    }
//}
//
//
//void sync0_isr(void * arg)
//{
//    DIG_process(DIG_PROCESS_APP_HOOK | DIG_PROCESS_INPUTS);
//    read_ack = ecat0->DC_SYNC0_STAT;
//}
//
//void ecat_isr(void * arg)
//{
//
//    ESC_read (ESCREG_LOCALTIME, (void *) &ESCvar.Time, sizeof (ESCvar.Time));
//    ESCvar.Time = etohl (ESCvar.Time);
//    ATOMIC_SET(ESCvar.ALevent, etohl(ecat0->AL_EVENT_REQ));
//
//    if(ESCvar.ALevent & ESCREG_ALEVENT_SM2)
//    {
//
//        if(dc_sync == 0)
//        {
//            DIG_process(DIG_PROCESS_OUTPUTS | DIG_PROCESS_APP_HOOK | DIG_PROCESS_INPUTS);
//        }
//        else
//        {
//            DIG_process(DIG_PROCESS_OUTPUTS);
//        }
//    }
//
//    if(ESCvar.ALevent & (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                         | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP))
//    {
//        /* Mask interrupts while servicing them */
//        ecat0->AL_EVENT_MASK &= ~(ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                                  | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP);
//        sem_signal(ecat_isr_sem);
//    }
//
//}
//#endif