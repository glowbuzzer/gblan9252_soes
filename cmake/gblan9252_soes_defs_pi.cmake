
#this is the process name we try and find to send signals to
SET(GBC_PROCESS_NAME "GBC")

#defines for which signal to send to GBC */
SET(SIGNAL_TO_SEND SIGIO)
#SET(SIGNAL_TO_SEND SIGPWR)
#RT signals require some jiggerpokery in GBC
#SET(SIGNAL_TO_SEND SIGRTMIN)


#N.B. SIGRTMIN is higher priority than SIGRTMAX if you are trying to use the RT signals */



SET(GBC_SHARED_MEMORY_NAME "gbc_shared_memory")

