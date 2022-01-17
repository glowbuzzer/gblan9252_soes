#include "esc_coe.h"
#include "utypes.h"
#include <stddef.h>
#include "ecat_options.h"
#ifndef HW_REV
#define HW_REV "1.0"
#endif

#ifndef SW_REV
#define SW_REV "1.0"
#endif

static const char acName1000[] = "Device Type";
static const char acName1008[] = "Device Name";
static const char acName1009[] = "Hardware Version";
static const char acName100A[] = "Software Version";
static const char acName1018[] = "Identity Object";
static const char acName1018_00[] = "Max SubIndex";
static const char acName1018_01[] = "Vendor ID";
static const char acName1018_02[] = "Product Code";
static const char acName1018_03[] = "Revision Number";
static const char acName1018_04[] = "Serial Number";
static const char acName1600[] = "Outputs";
static const char acName1600_00[] = "Max SubIndex";
//static const char acName1600_01[] = "LED0";
static const char acName1600_01[] = "Machine Status Word";
static const char acName1600_02[] = "Active Fault";
static const char acName1600_03[] = "Fault History";
static const char acName1600_04[] = "Heartbeat";
static const char acName1600_05[] = "Joint 0 Statusword";
static const char acName1600_06[] = "Joint 1 Statusword";
static const char acName1600_07[] = "Joint 2 Statusword";
static const char acName1600_08[] = "Joint 3 Statusword";
static const char acName1600_09[] = "Joint 4 Statusword";
static const char acName1600_10[] = "Joint 5 Statusword";
static const char acName1600_11[] = "Joint 6 Statusword";
static const char acName1600_12[] = "Joint 7 Statusword";
static const char acName1600_13[] = "Joint 8 Statusword";
static const char acName1600_14[] = "Joint 9 Statusword";
static const char acName1600_15[] = "Joint 0 Actual Position";
static const char acName1600_16[] = "Joint 1 Actual Position";
static const char acName1600_17[] = "Joint 2 Actual Position";
static const char acName1600_18[] = "Joint 3 Actual Position";
static const char acName1600_19[] = "Joint 4 Actual Position";
static const char acName1600_20[] = "Joint 5 Actual Position";
static const char acName1600_21[] = "Joint 6 Actual Position";
static const char acName1600_22[] = "Joint 7 Actual Position";
static const char acName1600_23[] = "Joint 8 Actual Position";
static const char acName1600_24[] = "Joint 9 Actual Position";
static const char acName1600_25[] = "Digital 0";
static const char acName1600_26[] = "Digital 1";
static const char acName1600_27[] = "Digital 2";
static const char acName1600_28[] = "Digital 3";
static const char acName1600_29[] = "Digital 4";
static const char acName1600_30[] = "Digital 5";
static const char acName1600_31[] = "Digital 6";
static const char acName1600_32[] = "Digital 7";
static const char acName1600_33[] = "Digital 8";
static const char acName1600_34[] = "Digital 9";
static const char acName1600_35[] = "Digital 10";
static const char acName1600_36[] = "Digital 11";
static const char acName1600_37[] = "Digital 12";
static const char acName1600_38[] = "Digital 13";
static const char acName1600_39[] = "Digital 14";
static const char acName1600_40[] = "Digital 15";
static const char acName1600_41[] = "Digital 16";
static const char acName1600_42[] = "Digital 17";
static const char acName1600_43[] = "Digital 18";
static const char acName1600_44[] = "Digital 19";
static const char acName1600_45[] = "Digital 20";
static const char acName1600_46[] = "Digital 21";
static const char acName1600_47[] = "Digital 22";
static const char acName1600_48[] = "Digital 23";
static const char acName1600_49[] = "Digital 24";
static const char acName1600_50[] = "Digital 25";
static const char acName1600_51[] = "Digital 26";
static const char acName1600_52[] = "Digital 27";
static const char acName1600_53[] = "Digital 28";
static const char acName1600_54[] = "Digital 29";
static const char acName1600_55[] = "Digital 30";
static const char acName1600_56[] = "Digital 31";
static const char acName1600_57[] = "Digital 32";
static const char acName1600_58[] = "Digital 33";
static const char acName1600_59[] = "Digital 34";
static const char acName1600_60[] = "Digital 35";
static const char acName1600_61[] = "Digital 36";
static const char acName1600_62[] = "Digital 37";
static const char acName1600_63[] = "Digital 38";
static const char acName1600_64[] = "Digital 39";
static const char acName1600_65[] = "Digital 40";
static const char acName1600_66[] = "Digital 41";
static const char acName1600_67[] = "Digital 42";
static const char acName1600_68[] = "Digital 43";
static const char acName1600_69[] = "Digital 44";
static const char acName1600_70[] = "Digital 45";
static const char acName1600_71[] = "Digital 46";
static const char acName1600_72[] = "Digital 47";
static const char acName1600_73[] = "Digital 48";
static const char acName1600_74[] = "Digital 49";
static const char acName1600_75[] = "Digital 50";
static const char acName1600_76[] = "Digital 51";
static const char acName1600_77[] = "Digital 52";
static const char acName1600_78[] = "Digital 53";
static const char acName1600_79[] = "Digital 54";
static const char acName1600_80[] = "Digital 55";
static const char acName1600_81[] = "Digital 56";
static const char acName1600_82[] = "Digital 57";
static const char acName1600_83[] = "Digital 58";
static const char acName1600_84[] = "Digital 59";
static const char acName1600_85[] = "Digital 60";
static const char acName1600_86[] = "Digital 61";
static const char acName1600_87[] = "Digital 62";
static const char acName1600_88[] = "Digital 63";
static const char acName1600_89[] = "Analog 0";
static const char acName1600_90[] = "Analog 1";
static const char acName1600_91[] = "Analog 2";
static const char acName1600_92[] = "Analog 3";
static const char acName1600_93[] = "Analog 4";
static const char acName1600_94[] = "Analog 5";
static const char acName1600_95[] = "Integer 0";
static const char acName1600_96[] = "Integer 1";


static const char acName1A00[] = "Inputs";
static const char acName1A00_00[] = "Max SubIndex";
//static const char acName1A00_01[] = "Button0";

static const char acName1A00_01[] = "Machine Control Word";
static const char acName1A00_02[] = "GBC Control Word";
static const char acName1A00_03[] = "HLC Control Word";
static const char acName1A00_04[] = "Heartbeat";
static const char acName1A00_05[] = "Joint 0 Controlword";
static const char acName1A00_06[] = "Joint 1 Controlword";
static const char acName1A00_07[] = "Joint 2 Controlword";
static const char acName1A00_08[] = "Joint 3 Controlword";
static const char acName1A00_09[] = "Joint 4 Controlword";
static const char acName1A00_10[] = "Joint 5 Controlword";
static const char acName1A00_11[] = "Joint 6 Controlword";
static const char acName1A00_12[] = "Joint 7 Controlword";
static const char acName1A00_13[] = "Joint 8 Controlword";
static const char acName1A00_14[] = "Joint 9 Controlword";
static const char acName1A00_15[] = "Joint 0 Set Position";
static const char acName1A00_16[] = "Joint 1 Set Position";
static const char acName1A00_17[] = "Joint 2 Set Position";
static const char acName1A00_18[] = "Joint 3 Set Position";
static const char acName1A00_19[] = "Joint 4 Set Position";
static const char acName1A00_20[] = "Joint 5 Set Position";
static const char acName1A00_21[] = "Joint 6 Set Position";
static const char acName1A00_22[] = "Joint 7 Set Position";
static const char acName1A00_23[] = "Joint 8 Set Position";
static const char acName1A00_24[] = "Joint 9 Set Position";
static const char acName1A00_25[] = "Digital 0";
static const char acName1A00_26[] = "Digital 1";
static const char acName1A00_27[] = "Digital 2";
static const char acName1A00_28[] = "Digital 3";
static const char acName1A00_29[] = "Digital 4";
static const char acName1A00_30[] = "Digital 5";
static const char acName1A00_31[] = "Digital 6";
static const char acName1A00_32[] = "Digital 7";
static const char acName1A00_33[] = "Digital 8";
static const char acName1A00_34[] = "Digital 9";
static const char acName1A00_35[] = "Digital 10";
static const char acName1A00_36[] = "Digital 11";
static const char acName1A00_37[] = "Digital 12";
static const char acName1A00_38[] = "Digital 13";
static const char acName1A00_39[] = "Digital 14";
static const char acName1A00_40[] = "Digital 15";
static const char acName1A00_41[] = "Digital 16";
static const char acName1A00_42[] = "Digital 17";
static const char acName1A00_43[] = "Digital 18";
static const char acName1A00_44[] = "Digital 19";
static const char acName1A00_45[] = "Digital 20";
static const char acName1A00_46[] = "Digital 21";
static const char acName1A00_47[] = "Digital 22";
static const char acName1A00_48[] = "Digital 23";
static const char acName1A00_49[] = "Digital 24";
static const char acName1A00_50[] = "Digital 25";
static const char acName1A00_51[] = "Digital 26";
static const char acName1A00_52[] = "Digital 27";
static const char acName1A00_53[] = "Digital 28";
static const char acName1A00_54[] = "Digital 29";
static const char acName1A00_55[] = "Digital 30";
static const char acName1A00_56[] = "Digital 31";
static const char acName1A00_57[] = "Digital 32";
static const char acName1A00_58[] = "Digital 33";
static const char acName1A00_59[] = "Digital 34";
static const char acName1A00_60[] = "Digital 35";
static const char acName1A00_61[] = "Digital 36";
static const char acName1A00_62[] = "Digital 37";
static const char acName1A00_63[] = "Digital 38";
static const char acName1A00_64[] = "Digital 39";
static const char acName1A00_65[] = "Digital 40";
static const char acName1A00_66[] = "Digital 41";
static const char acName1A00_67[] = "Digital 42";
static const char acName1A00_68[] = "Digital 43";
static const char acName1A00_69[] = "Digital 44";
static const char acName1A00_70[] = "Digital 45";
static const char acName1A00_71[] = "Digital 46";
static const char acName1A00_72[] = "Digital 47";
static const char acName1A00_73[] = "Digital 48";
static const char acName1A00_74[] = "Digital 49";
static const char acName1A00_75[] = "Digital 50";
static const char acName1A00_76[] = "Digital 51";
static const char acName1A00_77[] = "Digital 52";
static const char acName1A00_78[] = "Digital 53";
static const char acName1A00_79[] = "Digital 54";
static const char acName1A00_80[] = "Digital 55";
static const char acName1A00_81[] = "Digital 56";
static const char acName1A00_82[] = "Digital 57";
static const char acName1A00_83[] = "Digital 58";
static const char acName1A00_84[] = "Digital 59";
static const char acName1A00_85[] = "Digital 60";
static const char acName1A00_86[] = "Digital 61";
static const char acName1A00_87[] = "Digital 62";
static const char acName1A00_88[] = "Digital 63";
static const char acName1A00_89[] = "Analog 0";
static const char acName1A00_90[] = "Analog 1";
static const char acName1A00_91[] = "Analog 2";
static const char acName1A00_92[] = "Analog 3";
static const char acName1A00_93[] = "Analog 4";
static const char acName1A00_94[] = "Analog 5";
static const char acName1A00_95[] = "Integer 0";
static const char acName1A00_96[] = "Integer 1";


static const char acName1C00[] = "Sync Manager Communication Type";
static const char acName1C00_00[] = "Max SubIndex";
static const char acName1C00_01[] = "Communications Type SM0";
static const char acName1C00_02[] = "Communications Type SM1";
static const char acName1C00_03[] = "Communications Type SM2";
static const char acName1C00_04[] = "Communications Type SM3";
static const char acName1C12[] = "Sync Manager 2 PDO Assignment";
static const char acName1C12_00[] = "Max SubIndex";
static const char acName1C12_01[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName6000[] = "Inputs";
static const char acName6000_00[] = "Max SubIndex";
//static const char acName6000_01[] = "Button0";
static const char acName7000[] = "Outputs";
static const char acName7000_00[] = "Max SubIndex";
//static const char acName7000_01[] = "LED0";
static const char acName8000[] = "Parameters";
static const char acName8000_00[] = "Max SubIndex";
static const char acName8000_01[] = "Multiplier";

const _objd SDO1000[] =
        {
                {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000, 0x01901389, NULL},
        };
const _objd SDO1008[] =
        {
                {0x0, DTYPE_VISIBLE_STRING, 88, ATYPE_RO, acName1008, 0, "evb9252_dig"},
        };
const _objd SDO1009[] =
        {
                {0x0, DTYPE_VISIBLE_STRING, 0, ATYPE_RO, acName1009, 0, HW_REV},
        };
const _objd SDO100A[] =
        {
                {0x0, DTYPE_VISIBLE_STRING, 0, ATYPE_RO, acName100A, 0, SW_REV},
        };
const _objd SDO1018[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1018_00, 4,      NULL},
                {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_01, 0x1337, NULL},
                {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_02, 1234,   NULL},
                {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_03, 0,      NULL},
                {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_04, 0x00000000, &Obj.serial},
        };

const _objd SDO1600[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1600_00,   96,        NULL},
                {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01,   0x70000120, NULL},
                {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02,   0x70000220, NULL},
                {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_03,   0x70000320, NULL},
                {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_04,   0x70000420, NULL},
                {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_05,   0x70000510, NULL},
                {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_06,   0x70000610, NULL},
                {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_07,   0x70000710, NULL},
                {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_08,   0x70000810, NULL},
                {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_09,   0x70000910, NULL},
                {0x0A, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_10,  0x70000a10, NULL},
                {0x0B, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_11,  0x70000b10, NULL},
                {0x0C, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_12,  0x70000c10, NULL},
                {0x0D, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_13,  0x70000d10, NULL},
                {0x0E, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_14,  0x70000e10, NULL},
                {0x0F, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_15, 0x70000f20, NULL},
                {0x10, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_16, 0x70001020, NULL},
                {0x11, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_17, 0x70001120, NULL},
                {0x12, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_18, 0x70001220, NULL},
                {0x13, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_19, 0x70001320, NULL},
                {0x14, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_20, 0x70001420, NULL},
                {0x15, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_21, 0x70001520, NULL},
                {0x16, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_22, 0x70001620, NULL},
                {0x17, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_23, 0x70001720, NULL},
                {0x18, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_24, 0x70001820, NULL},
                {0x19, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_25, 0x70001901, NULL},
                {0x1A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_26, 0x70001a01, NULL},
                {0x1B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_27, 0x70001b01, NULL},
                {0x1C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_28, 0x70001c01, NULL},
                {0x1D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_29, 0x70001d01, NULL},
                {0x1E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_30, 0x70001e01, NULL},
                {0x1F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_31, 0x70001f01, NULL},
                {0x20, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_32, 0x70002001, NULL},
                {0x21, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_33, 0x70002101, NULL},
                {0x22, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_34, 0x70002201, NULL},
                {0x23, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_35, 0x70002301, NULL},
                {0x24, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_36, 0x70002401, NULL},
                {0x25, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_37, 0x70002501, NULL},
                {0x26, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_38, 0x70002601, NULL},
                {0x27, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_39, 0x70002701, NULL},
                {0x28, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_40, 0x70002801, NULL},
                {0x29, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_41, 0x70002901, NULL},
                {0x2A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_42, 0x70002a01, NULL},
                {0x2B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_43, 0x70002b01, NULL},
                {0x2C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_44, 0x70002c01, NULL},
                {0x2D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_45, 0x70002d01, NULL},
                {0x2E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_46, 0x70002e01, NULL},
                {0x2F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_47, 0x70002f01, NULL},
                {0x30, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_48, 0x70003001, NULL},
                {0x31, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_49, 0x70003101, NULL},
                {0x32, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_50, 0x70003201, NULL},
                {0x33, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_51, 0x70003301, NULL},
                {0x34, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_52, 0x70003401, NULL},
                {0x35, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_53, 0x70003501, NULL},
                {0x36, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_54, 0x70003601, NULL},
                {0x37, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_55, 0x70003701, NULL},
                {0x38, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_56, 0x70003801, NULL},
                {0x39, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_57, 0x70003901, NULL},
                {0x3A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_58, 0x70003a01, NULL},
                {0x3B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_59, 0x70003b01, NULL},
                {0x3C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_60, 0x70003c01, NULL},
                {0x3D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_61, 0x70003d01, NULL},
                {0x3E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_62, 0x70003e01, NULL},
                {0x3F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_63, 0x70003f01, NULL},
                {0x40, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_64, 0x70004001, NULL},
                {0x41, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_65, 0x70004101, NULL},
                {0x42, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_66, 0x70004201, NULL},
                {0x43, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_67, 0x70004301, NULL},
                {0x44, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_68, 0x70004401, NULL},
                {0x45, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_69, 0x70004501, NULL},
                {0x46, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_70, 0x70004601, NULL},
                {0x47, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_71, 0x70004701, NULL},
                {0x48, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_72, 0x70004801, NULL},
                {0x49, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_73, 0x70004901, NULL},
                {0x4A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_74, 0x70004a01, NULL},
                {0x4B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_75, 0x70004b01, NULL},
                {0x4C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_76, 0x70004c01, NULL},
                {0x4D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_77, 0x70004d01, NULL},
                {0x4E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_78, 0x70004e01, NULL},
                {0x4F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_79, 0x70004f01, NULL},
                {0x50, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_80, 0x70005001, NULL},
                {0x51, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_81, 0x70005101, NULL},
                {0x52, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_82, 0x70005201, NULL},
                {0x53, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_83, 0x70005301, NULL},
                {0x54, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_84, 0x70005401, NULL},
                {0x55, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_85, 0x70005501, NULL},
                {0x56, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_86, 0x70005601, NULL},
                {0x57, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_87, 0x70005701, NULL},
                {0x58, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1600_88, 0x70005801, NULL},
                {0x59, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_89, 0x70005920, NULL},
                {0x5A, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_90, 0x70005a20, NULL},
                {0x5B, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_91, 0x70005b20, NULL},
                {0x5C, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_92, 0x70005c20, NULL},
                {0x5D, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_93, 0x70005d20, NULL},
                {0x5E, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1600_94, 0x70005e20, NULL},
                {0x5F, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_95, 0x70005f20, NULL},
                {0x60, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1600_96, 0x70006020, NULL},


        };


const _objd SDO1A00[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1A00_00, 96,        NULL},
                {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60000120, NULL},
                {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60000220, NULL},
                {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_03, 0x60000320, NULL},
                {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_04, 0x60000420, NULL},
                {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_05, 0x60000510, NULL},
                {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_06, 0x60000610, NULL},
                {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_07, 0x60000710, NULL},
                {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_08, 0x60000810, NULL},
                {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_09, 0x60000910, NULL},
                {0x0A, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_10, 0x60000a10, NULL},
                {0x0B, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_11, 0x60000b10, NULL},
                {0x0C, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_12, 0x60000c10, NULL},
                {0x0D, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_13, 0x60000d10, NULL},
                {0x0E, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_14, 0x60000e10, NULL},
                {0x0F, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_15, 0x60000f20, NULL},
                {0x10, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_16, 0x60001020, NULL},
                {0x11, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_17, 0x60001120, NULL},
                {0x12, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_18, 0x60001220, NULL},
                {0x13, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_19, 0x60001320, NULL},
                {0x14, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_20, 0x60001420, NULL},
                {0x15, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_21, 0x60001520, NULL},
                {0x32, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_22, 0x60001620, NULL},
                {0x17, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_23, 0x60001720, NULL},
                {0x18, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_24, 0x60001820, NULL},
                {0x19, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_25, 0x60001901, NULL},
                {0x1A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_26, 0x60001a01, NULL},
                {0x1B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_27, 0x60001b01, NULL},
                {0x1C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_28, 0x60001c01, NULL},
                {0x1D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_29, 0x60001d01, NULL},
                {0x1E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_30, 0x60001e01, NULL},
                {0x1F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_31, 0x60001f01, NULL},
                {0x20, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_32, 0x60002001, NULL},
                {0x21, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_33, 0x60002101, NULL},
                {0x22, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_34, 0x60002201, NULL},
                {0x23, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_35, 0x60002301, NULL},
                {0x24, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_36, 0x60002401, NULL},
                {0x25, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_37, 0x60002501, NULL},
                {0x26, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_38, 0x60002601, NULL},
                {0x27, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_39, 0x60002701, NULL},
                {0x28, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_40, 0x60002801, NULL},
                {0x29, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_41, 0x60002901, NULL},
                {0x2A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_42, 0x60002a01, NULL},
                {0x2B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_43, 0x60002b01, NULL},
                {0x2C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_44, 0x60002c01, NULL},
                {0x2D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_45, 0x60002d01, NULL},
                {0x2E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_46, 0x60002e01, NULL},
                {0x2F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_47, 0x60002f01, NULL},
                {0x30, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_48, 0x60003001, NULL},
                {0x31, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_49, 0x60003101, NULL},
                {0x32, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_50, 0x60003201, NULL},
                {0x33, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_51, 0x60003301, NULL},
                {0x34, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_52, 0x60003401, NULL},
                {0x35, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_53, 0x60003501, NULL},
                {0x36, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_54, 0x60003601, NULL},
                {0x37, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_55, 0x60003701, NULL},
                {0x38, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_56, 0x60003801, NULL},
                {0x39, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_57, 0x60003901, NULL},
                {0x3A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_58, 0x60003a01, NULL},
                {0x3B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_59, 0x60003b01, NULL},
                {0x3C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_60, 0x60003c01, NULL},
                {0x3D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_61, 0x60003d01, NULL},
                {0x3E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_62, 0x60003e01, NULL},
                {0x3F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_63, 0x60003f01, NULL},
                {0x40, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_64, 0x60004001, NULL},
                {0x41, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_65, 0x60004101, NULL},
                {0x42, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_66, 0x60004201, NULL},
                {0x43, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_67, 0x60004301, NULL},
                {0x44, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_68, 0x60004401, NULL},
                {0x45, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_69, 0x60004501, NULL},
                {0x46, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_70, 0x60004601, NULL},
                {0x47, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_71, 0x60004701, NULL},
                {0x48, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_72, 0x60004801, NULL},
                {0x49, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_73, 0x60004901, NULL},
                {0x4A, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_74, 0x60004a01, NULL},
                {0x4B, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_75, 0x60004b01, NULL},
                {0x4C, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_76, 0x60004c01, NULL},
                {0x4D, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_77, 0x60004d01, NULL},
                {0x4E, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_78, 0x60004e01, NULL},
                {0x4F, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_79, 0x60004f01, NULL},
                {0x50, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_80, 0x60005001, NULL},
                {0x51, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_81, 0x60005101, NULL},
                {0x52, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_82, 0x60005201, NULL},
                {0x53, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_83, 0x60005301, NULL},
                {0x54, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_84, 0x60005401, NULL},
                {0x55, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_85, 0x60005501, NULL},
                {0x56, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_86, 0x60005601, NULL},
                {0x57, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_87, 0x60005701, NULL},
                {0x58, DTYPE_UNSIGNED32,    32,  ATYPE_RO, acName1A00_88, 0x60005801, NULL},
                {0x59, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_89, 0x60005920, NULL},
                {0x5A, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_90, 0x60005a20, NULL},
                {0x5B, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_91, 0x60005b20, NULL},
                {0x5C, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_92, 0x60005c20, NULL},
                {0x5D, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_93, 0x60005d20, NULL},
                {0x5E, DTYPE_UNSIGNED32,     32, ATYPE_RO, acName1A00_94, 0x60005e20, NULL},
                {0x5F, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_95, 0x60005f20, NULL},
                {0x60, DTYPE_UNSIGNED32,  32, ATYPE_RO, acName1A00_96, 0x60006020, NULL},
        };




const _objd SDO1C00[] =
        {
                {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_00, 4, NULL},
                {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_01, 1, NULL},
                {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_02, 2, NULL},
                {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_03, 3, NULL},
                {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_04, 4, NULL},
        };
const _objd SDO1C12[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1C12_00, 1,      NULL},
                {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
        };
const _objd SDO1C13[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1C13_00, 1,      NULL},
                {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
        };


static const char acName1C32[] = "SyncMgrParam";
static const char acName1C32_00[] = "Number of Elements";
static const char acName1C32_01[] = "SyncType";
static const char acName1C32_02[] = "CycleTime";
static const char acName1C32_03[] = "ShiftTime";
static const char acName1C32_04[] = "SyncTypeSupport";
static const char acName1C32_05[] = "MinCycleTime";
static const char acName1C32_06[] = "CalcCopyTime";
static const char acName1C32_07[] = "MinDelayTime";
static const char acName1C32_08[] = "GetCycleTime";
static const char acName1C32_09[] = "DelayTime";
static const char acName1C32_0A[] = "Sync0CycleTime";
static const char acName1C32_0B[] = "SMEventMissedCnt";
static const char acName1C32_0C[] = "CycleTimeTooSmallCnt";
static const char acName1C32_0D[] = "ShiftTimeTooSmallCnt";
static const char acName1C32_0E[] = "RxPDOToggleFailed";
static const char acName1C32_0F[] = "MinCycleDist";
static const char acName1C32_10[] = "MaxCycleDist";
static const char acName1C32_11[] = "MinSMSYNCDist";
static const char acName1C32_12[] = "MaxSMSYNCDist";
static const char acName1C32_18[] = "Reserved0x18";
static const char acName1C32_1C[] = "Reserved0x1C";
static const char acName1C32_1D[] = "Reserved0x1D";
static const char acName1C32_1E[] = "Reserved0x1E";
static const char acName1C32_1F[] = "Reserved0x1F";
static const char acName1C32_20[] = "SyncError";

const _objd SDO1C32[] =
        {
                {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_00, 24, NULL},
                {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_01, 1, &Mb.SyncMgrParam.SyncType},
                {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_02, 0, &Mb.SyncMgrParam.CycleTime},
                {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_03, 0, &Mb.SyncMgrParam.ShiftTime},
                {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_04, 0x6, &Mb.SyncMgrParam.SyncTypeSupport},
                {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_05, 0, &Mb.SyncMgrParam.MinCycleTime},
                {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_06, 0, &Mb.SyncMgrParam.CalcCopyTime},
                {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_07, 0, &Mb.SyncMgrParam.MinDelayTime},
                {0x08, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_08, 0, &Mb.SyncMgrParam.GetCycleTime},
                {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_09, 0, &Mb.SyncMgrParam.DelayTime},
                {0x0A, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_0A, 0, &Mb.SyncMgrParam.Sync0CycleTime},
                {0x0B, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0B, 0, &Mb.SyncMgrParam.SMEventMissedCnt},
                {0x0C, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0C, 0, &Mb.SyncMgrParam.CycleTimeTooSmallCnt},
                {0x0D, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0D, 0, &Mb.SyncMgrParam.ShiftTimeTooSmallCnt},
                {0x0E, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C32_0E, 0, &Mb.SyncMgrParam.RxPDOToggleFailed},
                {0x0F, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_0F, 0, &Mb.SyncMgrParam.MinCycleDist},
                {0x10, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_10, 0, &Mb.SyncMgrParam.MaxCycleDist},
                {0x11, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_11, 0, &Mb.SyncMgrParam.MinSMSYNCDist},
                {0x12, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1C32_12, 0, &Mb.SyncMgrParam.MaxSMSYNCDist},
                {0x18, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_18, 0, &Mb.SyncMgrParam.Reserved0x18},
                {0x1C, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_1C, 0, &Mb.SyncMgrParam.Reserved0x1C},
                {0x1D, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_1D, 0, &Mb.SyncMgrParam.Reserved0x1D},
                {0x1E, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_1E, 0, &Mb.SyncMgrParam.Reserved0x1E},
                {0x1F, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_1F, 0, &Mb.SyncMgrParam.Reserved0x1F},
                {0x20, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C32_20, 0, &Mb.SyncMgrParam.SyncError},
        };

static const char acName6000_01[] = "Machine Control Word";
static const char acName6000_02[] = "GBC Control Word";
static const char acName6000_03[] = "HLC Control Word";
static const char acName6000_04[] = "Heartbeat";
static const char acName6000_05[] = "Joint 0 Controlword";
static const char acName6000_06[] = "Joint 1 Controlword";
static const char acName6000_07[] = "Joint 2 Controlword";
static const char acName6000_08[] = "Joint 3 Controlword";
static const char acName6000_09[] = "Joint 4 Controlword";
static const char acName6000_10[] = "Joint 5 Controlword";
static const char acName6000_11[] = "Joint 6 Controlword";
static const char acName6000_12[] = "Joint 7 Controlword";
static const char acName6000_13[] = "Joint 8 Controlword";
static const char acName6000_14[] = "Joint 9 Controlword";
static const char acName6000_15[] = "Joint 0 Set Position";
static const char acName6000_16[] = "Joint 1 Set Position";
static const char acName6000_17[] = "Joint 2 Set Position";
static const char acName6000_18[] = "Joint 3 Set Position";
static const char acName6000_19[] = "Joint 4 Set Position";
static const char acName6000_20[] = "Joint 5 Set Position";
static const char acName6000_21[] = "Joint 6 Set Position";
static const char acName6000_22[] = "Joint 7 Set Position";
static const char acName6000_23[] = "Joint 8 Set Position";
static const char acName6000_24[] = "Joint 9 Set Position";
static const char acName6000_25[] = "Digital 0";
static const char acName6000_26[] = "Digital 1";
static const char acName6000_27[] = "Digital 2";
static const char acName6000_28[] = "Digital 3";
static const char acName6000_29[] = "Digital 4";
static const char acName6000_30[] = "Digital 5";
static const char acName6000_31[] = "Digital 6";
static const char acName6000_32[] = "Digital 7";
static const char acName6000_33[] = "Digital 8";
static const char acName6000_34[] = "Digital 9";
static const char acName6000_35[] = "Digital 10";
static const char acName6000_36[] = "Digital 11";
static const char acName6000_37[] = "Digital 12";
static const char acName6000_38[] = "Digital 13";
static const char acName6000_39[] = "Digital 14";
static const char acName6000_40[] = "Digital 15";
static const char acName6000_41[] = "Digital 16";
static const char acName6000_42[] = "Digital 17";
static const char acName6000_43[] = "Digital 18";
static const char acName6000_44[] = "Digital 19";
static const char acName6000_45[] = "Digital 20";
static const char acName6000_46[] = "Digital 21";
static const char acName6000_47[] = "Digital 22";
static const char acName6000_48[] = "Digital 23";
static const char acName6000_49[] = "Digital 24";
static const char acName6000_50[] = "Digital 25";
static const char acName6000_51[] = "Digital 26";
static const char acName6000_52[] = "Digital 27";
static const char acName6000_53[] = "Digital 28";
static const char acName6000_54[] = "Digital 29";
static const char acName6000_55[] = "Digital 30";
static const char acName6000_56[] = "Digital 31";
static const char acName6000_57[] = "Digital 32";
static const char acName6000_58[] = "Digital 33";
static const char acName6000_59[] = "Digital 34";
static const char acName6000_60[] = "Digital 35";
static const char acName6000_61[] = "Digital 36";
static const char acName6000_62[] = "Digital 37";
static const char acName6000_63[] = "Digital 38";
static const char acName6000_64[] = "Digital 39";
static const char acName6000_65[] = "Digital 40";
static const char acName6000_66[] = "Digital 41";
static const char acName6000_67[] = "Digital 42";
static const char acName6000_68[] = "Digital 43";
static const char acName6000_69[] = "Digital 44";
static const char acName6000_70[] = "Digital 45";
static const char acName6000_71[] = "Digital 46";
static const char acName6000_72[] = "Digital 47";
static const char acName6000_73[] = "Digital 48";
static const char acName6000_74[] = "Digital 49";
static const char acName6000_75[] = "Digital 50";
static const char acName6000_76[] = "Digital 51";
static const char acName6000_77[] = "Digital 52";
static const char acName6000_78[] = "Digital 53";
static const char acName6000_79[] = "Digital 54";
static const char acName6000_80[] = "Digital 55";
static const char acName6000_81[] = "Digital 56";
static const char acName6000_82[] = "Digital 57";
static const char acName6000_83[] = "Digital 58";
static const char acName6000_84[] = "Digital 59";
static const char acName6000_85[] = "Digital 60";
static const char acName6000_86[] = "Digital 61";
static const char acName6000_87[] = "Digital 62";
static const char acName6000_88[] = "Digital 63";
static const char acName6000_89[] = "Analog 0";
static const char acName6000_90[] = "Analog 1";
static const char acName6000_91[] = "Analog 2";
static const char acName6000_92[] = "Analog 3";
static const char acName6000_93[] = "Analog 4";
static const char acName6000_94[] = "Analog 5";
static const char acName6000_95[] = "Integer 0";
static const char acName6000_96[] = "Integer 1";

const _objd SDO6000[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName6000_00, 96,             NULL},
                {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_01, 0, &Obj.in.machine_control_word},
                {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_02, 0, &Obj.in.gbc_controlword},
                {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_03, 0, &Obj.in.hlc_controlword},
                {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_04, 0, &Obj.in.heartbeat},
                {0x05, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_05, 0, &Obj.in.joint_0_controlword},
                {0x06, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_06, 0, &Obj.in.joint_1_controlword},
                {0x07, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_07, 0, &Obj.in.joint_2_controlword},
                {0x08, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_08, 0, &Obj.in.joint_3_controlword},
                {0x09, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_09, 0, &Obj.in.joint_4_controlword},
                {0x0A, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_10, 0, &Obj.in.joint_5_controlword},
                {0x0B, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_11, 0, &Obj.in.joint_6_controlword},
                {0x0C, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_12, 0, &Obj.in.joint_7_controlword},
                {0x0D, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_13, 0, &Obj.in.joint_8_controlword},
                {0x0E, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_TXPDO, acName6000_14, 0, &Obj.in.joint_9_controlword},
                {0x0F, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_15, 0, &Obj.in.joint_0_set_position},
                {0x10, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_16, 0, &Obj.in.joint_1_set_position},
                {0x11, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_17, 0, &Obj.in.joint_2_set_position},
                {0x12, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_18, 0, &Obj.in.joint_3_set_position},
                {0x13, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_19, 0, &Obj.in.joint_4_set_position},
                {0x14, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_20, 0, &Obj.in.joint_5_set_position},
                {0x15, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_21, 0, &Obj.in.joint_6_set_position},
                {0x16, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_22, 0, &Obj.in.joint_7_set_position},
                {0x17, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_23, 0, &Obj.in.joint_8_set_position},
                {0x18, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_24, 0, &Obj.in.joint_9_set_position},
                {0x19, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_25, 0, &Obj.in.digital_0},
                {0x1A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_26, 0, &Obj.in.digital_1},
                {0x1B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_27, 0, &Obj.in.digital_2},
                {0x1C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_28, 0, &Obj.in.digital_3},
                {0x1D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_29, 0, &Obj.in.digital_4},
                {0x1E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_30, 0, &Obj.in.digital_5},
                {0x1F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_31, 1, &Obj.in.digital_6},
                {0x20, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_32, 0, &Obj.in.digital_7},
                {0x21, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_33, 0, &Obj.in.digital_8},
                {0x22, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_34, 0, &Obj.in.digital_9},
                {0x23, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_35, 0, &Obj.in.digital_10},
                {0x24, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_36, 0, &Obj.in.digital_11},
                {0x25, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_37, 0, &Obj.in.digital_12},
                {0x26, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_38, 0, &Obj.in.digital_13},
                {0x27, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_39, 0, &Obj.in.digital_14},
                {0x28, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_40, 0, &Obj.in.digital_15},
                {0x29, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_41, 0, &Obj.in.digital_16},
                {0x2A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_42, 0, &Obj.in.digital_17},
                {0x2B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_43, 0, &Obj.in.digital_18},
                {0x2C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_44, 0, &Obj.in.digital_19},
                {0x2D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_45, 0, &Obj.in.digital_20},
                {0x2E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_46, 0, &Obj.in.digital_21},
                {0x2F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_47, 0, &Obj.in.digital_22},
                {0x30, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_48, 0, &Obj.in.digital_23},
                {0x31, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_49, 0, &Obj.in.digital_24},
                {0x32, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_50, 0, &Obj.in.digital_25},
                {0x33, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_51, 0, &Obj.in.digital_26},
                {0x34, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_52, 0, &Obj.in.digital_27},
                {0x35, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_53, 0, &Obj.in.digital_28},
                {0x36, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_54, 0, &Obj.in.digital_29},
                {0x37, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_55, 0, &Obj.in.digital_30},
                {0x38, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_56, 0, &Obj.in.digital_31},
                {0x39, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_57, 0, &Obj.in.digital_32},
                {0x3A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_58, 0, &Obj.in.digital_33},
                {0x3B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_59, 0, &Obj.in.digital_34},
                {0x3C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_60, 0, &Obj.in.digital_35},
                {0x3D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_61, 0, &Obj.in.digital_36},
                {0x3E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_62, 0, &Obj.in.digital_37},
                {0x3F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_63, 0, &Obj.in.digital_38},
                {0x40, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_64, 0, &Obj.in.digital_39},
                {0x41, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_65, 0, &Obj.in.digital_40},
                {0x42, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_66, 0, &Obj.in.digital_41},
                {0x43, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_67, 0, &Obj.in.digital_42},
                {0x44, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_68, 0, &Obj.in.digital_43},
                {0x45, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_69, 0, &Obj.in.digital_44},
                {0x46, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_70, 0, &Obj.in.digital_45},
                {0x47, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_71, 0, &Obj.in.digital_46},
                {0x48, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_72, 0, &Obj.in.digital_47},
                {0x49, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_73, 0, &Obj.in.digital_48},
                {0x4A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_74, 0, &Obj.in.digital_49},
                {0x4B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_75, 0, &Obj.in.digital_50},
                {0x4C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_76, 0, &Obj.in.digital_51},
                {0x4D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_77, 0, &Obj.in.digital_52},
                {0x4E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_78, 0, &Obj.in.digital_53},
                {0x4F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_79, 0, &Obj.in.digital_54},
                {0x50, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_80, 0, &Obj.in.digital_55},
                {0x51, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_81, 0, &Obj.in.digital_56},
                {0x52, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_82, 0, &Obj.in.digital_57},
                {0x53, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_83, 0, &Obj.in.digital_58},
                {0x54, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_84, 0, &Obj.in.digital_59},
                {0x55, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_85, 0, &Obj.in.digital_60},
                {0x56, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_86, 0, &Obj.in.digital_61},
                {0x57, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_87, 0, &Obj.in.digital_62},
                {0x58, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_TXPDO, acName6000_88, 0, &Obj.in.digital_63},
                {0x59, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_89, 0, &Obj.in.analog_0},
                {0x5A, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_90, 0, &Obj.in.analog_1},
                {0x5B, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_91, 0, &Obj.in.analog_2},
                {0x5C, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_92, 0, &Obj.in.analog_3},
                {0x5D, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_93, 0, &Obj.in.analog_4},
                {0x5E, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_TXPDO, acName6000_94, 0, &Obj.in.analog_5},
                {0x5F, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_95, 0, &Obj.in.integer_0},
                {0x60, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_TXPDO, acName6000_96, 0, &Obj.in.integer_1},
        };


//
//const _objd SDO6000[] =
//        {
//                {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_00, 7,               NULL},
//                {0x01, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_01, 0, &Obj.Buttons.Button0},
//                {0x02, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_02, 0, &Obj.Buttons.Button1},
//                {0x03, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_03, 0, &Obj.Buttons.Button2},
//                {0x04, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_04, 0, &Obj.Buttons.Button3},
//                {0x05, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_05, 0, &Obj.Buttons.Button4},
//                {0x06, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_06, 0, &Obj.Buttons.Button5},
//                {0x07, DTYPE_BOOLEAN,   1, ATYPE_RO | ATYPE_TXPDO, acName6000_07, 0, &Obj.Buttons.Button6},
//
//        };





//const _objd SDO7000[] =
//{
//  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_00, 7, NULL},
//  {0x01, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_01, 0, &Obj.LEDs.LED0},
//  {0x02, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_02, 0, &Obj.LEDs.LED1},
//  {0x03, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_03, 0, &Obj.LEDs.LED2},
//  {0x04, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_04, 0, &Obj.LEDs.LED3},
//  {0x05, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_05, 0, &Obj.LEDs.LED4},
//  {0x06, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_06, 0, &Obj.LEDs.LED5},
//  {0x07, DTYPE_BOOLEAN, 1, ATYPE_RO | ATYPE_RXPDO, acName7000_07, 0, &Obj.LEDs.LED6},
//};

static const char acName7000_01[] = "Machine Status Word";
static const char acName7000_02[] = "Active Fault";
static const char acName7000_03[] = "Fault History";
static const char acName7000_04[] = "Heartbeat";
static const char acName7000_05[] = "Joint 0 Statusword";
static const char acName7000_06[] = "Joint 1 Statusword";
static const char acName7000_07[] = "Joint 2 Statusword";
static const char acName7000_08[] = "Joint 3 Statusword";
static const char acName7000_09[] = "Joint 4 Statusword";
static const char acName7000_10[] = "Joint 5 Statusword";
static const char acName7000_11[] = "Joint 6 Statusword";
static const char acName7000_12[] = "Joint 7 Statusword";
static const char acName7000_13[] = "Joint 8 Statusword";
static const char acName7000_14[] = "Joint 9 Statusword";
static const char acName7000_15[] = "Joint 0 Actual Position";
static const char acName7000_16[] = "Joint 1 Actual Position";
static const char acName7000_17[] = "Joint 2 Actual Position";
static const char acName7000_18[] = "Joint 3 Actual Position";
static const char acName7000_19[] = "Joint 4 Actual Position";
static const char acName7000_20[] = "Joint 5 Actual Position";
static const char acName7000_21[] = "Joint 6 Actual Position";
static const char acName7000_22[] = "Joint 7 Actual Position";
static const char acName7000_23[] = "Joint 8 Actual Position";
static const char acName7000_24[] = "Joint 9 Actual Position";
static const char acName7000_25[] = "Digital 0";
static const char acName7000_26[] = "Digital 1";
static const char acName7000_27[] = "Digital 2";
static const char acName7000_28[] = "Digital 3";
static const char acName7000_29[] = "Digital 4";
static const char acName7000_30[] = "Digital 5";
static const char acName7000_31[] = "Digital 6";
static const char acName7000_32[] = "Digital 7";
static const char acName7000_33[] = "Digital 8";
static const char acName7000_34[] = "Digital 9";
static const char acName7000_35[] = "Digital 10";
static const char acName7000_36[] = "Digital 11";
static const char acName7000_37[] = "Digital 12";
static const char acName7000_38[] = "Digital 13";
static const char acName7000_39[] = "Digital 14";
static const char acName7000_40[] = "Digital 15";
static const char acName7000_41[] = "Digital 16";
static const char acName7000_42[] = "Digital 17";
static const char acName7000_43[] = "Digital 18";
static const char acName7000_44[] = "Digital 19";
static const char acName7000_45[] = "Digital 20";
static const char acName7000_46[] = "Digital 21";
static const char acName7000_47[] = "Digital 22";
static const char acName7000_48[] = "Digital 23";
static const char acName7000_49[] = "Digital 24";
static const char acName7000_50[] = "Digital 25";
static const char acName7000_51[] = "Digital 26";
static const char acName7000_52[] = "Digital 27";
static const char acName7000_53[] = "Digital 28";
static const char acName7000_54[] = "Digital 29";
static const char acName7000_55[] = "Digital 30";
static const char acName7000_56[] = "Digital 31";
static const char acName7000_57[] = "Digital 32";
static const char acName7000_58[] = "Digital 33";
static const char acName7000_59[] = "Digital 34";
static const char acName7000_60[] = "Digital 35";
static const char acName7000_61[] = "Digital 36";
static const char acName7000_62[] = "Digital 37";
static const char acName7000_63[] = "Digital 38";
static const char acName7000_64[] = "Digital 39";
static const char acName7000_65[] = "Digital 40";
static const char acName7000_66[] = "Digital 41";
static const char acName7000_67[] = "Digital 42";
static const char acName7000_68[] = "Digital 43";
static const char acName7000_69[] = "Digital 44";
static const char acName7000_70[] = "Digital 45";
static const char acName7000_71[] = "Digital 46";
static const char acName7000_72[] = "Digital 47";
static const char acName7000_73[] = "Digital 48";
static const char acName7000_74[] = "Digital 49";
static const char acName7000_75[] = "Digital 50";
static const char acName7000_76[] = "Digital 51";
static const char acName7000_77[] = "Digital 52";
static const char acName7000_78[] = "Digital 53";
static const char acName7000_79[] = "Digital 54";
static const char acName7000_80[] = "Digital 55";
static const char acName7000_81[] = "Digital 56";
static const char acName7000_82[] = "Digital 57";
static const char acName7000_83[] = "Digital 58";
static const char acName7000_84[] = "Digital 59";
static const char acName7000_85[] = "Digital 60";
static const char acName7000_86[] = "Digital 61";
static const char acName7000_87[] = "Digital 62";
static const char acName7000_88[] = "Digital 63";
static const char acName7000_89[] = "Analog 0";
static const char acName7000_90[] = "Analog 1";
static const char acName7000_91[] = "Analog 2";
static const char acName7000_92[] = "Analog 3";
static const char acName7000_93[] = "Analog 4";
static const char acName7000_94[] = "Analog 5";
static const char acName7000_95[] = "Integer 0";
static const char acName7000_96[] = "Integer 1";

const _objd SDO7000[] = {
        {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName7000_00, 96,             NULL},
        {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_RXPDO, acName7000_01, 0, &Obj.out.machine_status_word},
        {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_RXPDO, acName7000_02, 0, &Obj.out.active_fault},
        {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_RXPDO, acName7000_03, 0, &Obj.out.fault_history},
        {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_RXPDO, acName7000_04, 0, &Obj.out.heartbeat},
        {0x05, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_05, 0, &Obj.out.joint_0_statusword},
        {0x06, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_06, 0, &Obj.out.joint_1_statusword},
        {0x07, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_07, 0, &Obj.out.joint_2_statusword},
        {0x08, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_08, 0, &Obj.out.joint_3_statusword},
        {0x09, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_09, 0, &Obj.out.joint_4_statusword},
        {0x0A, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_10, 0, &Obj.out.joint_5_statusword},
        {0x0B, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_11, 0, &Obj.out.joint_6_statusword},
        {0x0C, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_12, 0, &Obj.out.joint_7_statusword},
        {0x0D, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_13, 0, &Obj.out.joint_8_statusword},
        {0x0E, DTYPE_UNSIGNED16, 16, ATYPE_RO | ATYPE_RXPDO, acName7000_14, 0, &Obj.out.joint_9_statusword},
        {0x0F, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_15, 0, &Obj.out.joint_0_actual_position},
        {0x10, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_16, 0, &Obj.out.joint_1_actual_position},
        {0x11, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_17, 0, &Obj.out.joint_2_actual_position},
        {0x12, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_18, 0, &Obj.out.joint_3_actual_position},
        {0x13, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_19, 0, &Obj.out.joint_4_actual_position},
        {0x14, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_20, 0, &Obj.out.joint_5_actual_position},
        {0x15, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_21, 0, &Obj.out.joint_6_actual_position},
        {0x16, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_22, 0, &Obj.out.joint_7_actual_position},
        {0x17, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_23, 0, &Obj.out.joint_8_actual_position},
        {0x18, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_24, 0, &Obj.out.joint_9_actual_position},
        {0x19, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_25, 1, &Obj.out.digital_0},
        {0x1A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_26, 0, &Obj.out.digital_1},
        {0x1B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_27, 0, &Obj.out.digital_2},
        {0x1C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_28, 0, &Obj.out.digital_3},
        {0x1D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_29, 0, &Obj.out.digital_4},
        {0x1E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_30, 0, &Obj.out.digital_5},
        {0x1F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_31, 0, &Obj.out.digital_6},
        {0x20, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_32, 0, &Obj.out.digital_7},
        {0x21, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_33, 0, &Obj.out.digital_8},
        {0x22, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_34, 0, &Obj.out.digital_9},
        {0x23, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_35, 0, &Obj.out.digital_10},
        {0x24, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_36, 0, &Obj.out.digital_11},
        {0x25, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_37, 0, &Obj.out.digital_12},
        {0x26, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_38, 0, &Obj.out.digital_13},
        {0x27, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_39, 0, &Obj.out.digital_14},
        {0x28, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_40, 0, &Obj.out.digital_15},
        {0x29, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_41, 0, &Obj.out.digital_16},
        {0x2A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_42, 0, &Obj.out.digital_17},
        {0x2B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_43, 0, &Obj.out.digital_18},
        {0x2C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_44, 0, &Obj.out.digital_19},
        {0x2D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_45, 0, &Obj.out.digital_20},
        {0x2E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_46, 0, &Obj.out.digital_21},
        {0x2F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_47, 0, &Obj.out.digital_22},
        {0x30, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_48, 0, &Obj.out.digital_23},
        {0x31, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_49, 0, &Obj.out.digital_24},
        {0x32, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_50, 0, &Obj.out.digital_25},
        {0x33, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_51, 0, &Obj.out.digital_26},
        {0x34, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_52, 0, &Obj.out.digital_27},
        {0x35, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_53, 0, &Obj.out.digital_28},
        {0x36, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_54, 0, &Obj.out.digital_29},
        {0x37, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_55, 0, &Obj.out.digital_30},
        {0x38, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_56, 0, &Obj.out.digital_31},
        {0x39, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_57, 0, &Obj.out.digital_32},
        {0x3A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_58, 0, &Obj.out.digital_33},
        {0x3B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_59, 0, &Obj.out.digital_34},
        {0x3C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_60, 0, &Obj.out.digital_35},
        {0x3D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_61, 0, &Obj.out.digital_36},
        {0x3E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_62, 0, &Obj.out.digital_37},
        {0x3F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_63, 0, &Obj.out.digital_38},
        {0x40, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_64, 0, &Obj.out.digital_39},
        {0x41, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_65, 0, &Obj.out.digital_40},
        {0x42, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_66, 0, &Obj.out.digital_41},
        {0x43, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_67, 0, &Obj.out.digital_42},
        {0x44, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_68, 0, &Obj.out.digital_43},
        {0x45, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_69, 0, &Obj.out.digital_44},
        {0x46, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_70, 0, &Obj.out.digital_45},
        {0x47, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_71, 0, &Obj.out.digital_46},
        {0x48, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_72, 0, &Obj.out.digital_47},
        {0x49, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_73, 0, &Obj.out.digital_48},
        {0x4A, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_74, 0, &Obj.out.digital_49},
        {0x4B, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_75, 0, &Obj.out.digital_50},
        {0x4C, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_76, 0, &Obj.out.digital_51},
        {0x4D, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_77, 0, &Obj.out.digital_52},
        {0x4E, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_78, 0, &Obj.out.digital_53},
        {0x4F, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_79, 0, &Obj.out.digital_54},
        {0x50, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_80, 0, &Obj.out.digital_55},
        {0x51, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_81, 0, &Obj.out.digital_56},
        {0x52, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_82, 0, &Obj.out.digital_57},
        {0x53, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_83, 0, &Obj.out.digital_58},
        {0x54, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_84, 0, &Obj.out.digital_59},
        {0x55, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_85, 0, &Obj.out.digital_60},
        {0x56, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_86, 0, &Obj.out.digital_61},
        {0x57, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_87, 0, &Obj.out.digital_62},
        {0x58, DTYPE_BOOLEAN,    1,  ATYPE_RO | ATYPE_RXPDO, acName7000_88, 0, &Obj.out.digital_63},
        {0x59, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_89, 0, &Obj.out.analog_0},
        {0x5A, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_90, 0, &Obj.out.analog_1},
        {0x5B, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_91, 0, &Obj.out.analog_2},
        {0x5C, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_92, 0, &Obj.out.analog_3},
        {0x5D, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_93, 0, &Obj.out.analog_4},
        {0x5E, DTYPE_REAL32,     32, ATYPE_RO | ATYPE_RXPDO, acName7000_94, 0, &Obj.out.analog_5},
        {0x5F, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_95, 0, &Obj.out.integer_0},
        {0x60, DTYPE_INTEGER32,  32, ATYPE_RO | ATYPE_RXPDO, acName7000_96, 0, &Obj.out.integer_1},
};



const _objd SDO8000[] =
        {
                {0x00, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName8000_00, 1, NULL},
                {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RW, acName8000_01, 0, &Obj.Parameters.Multiplier},
        };

const _objectlist SDOobjects[] =
        {
                {0x1000, OTYPE_VAR,    0,    0, acName1000, SDO1000},
                {0x1008, OTYPE_VAR,    0,    0, acName1008, SDO1008},
                {0x1009, OTYPE_VAR,    0,    0, acName1009, SDO1009},
                {0x100A, OTYPE_VAR,    0,    0, acName100A, SDO100A},
                {0x1018, OTYPE_RECORD, 4,    0, acName1018, SDO1018},
                {0x1600, OTYPE_RECORD, 96,    0, acName1600, SDO1600},
                {0x1A00, OTYPE_RECORD, 96,    0, acName1A00, SDO1A00},
                {0x1C00, OTYPE_ARRAY,  4,    0, acName1C00, SDO1C00},
                {0x1C12, OTYPE_ARRAY,  1,    0, acName1C12, SDO1C12},
                {0x1C13, OTYPE_ARRAY,  1,    0, acName1C13, SDO1C13},
#if SOES_IRQ == 1
                {0x1C32, OTYPE_RECORD, 32, 0, acName1C32, SDO1C32},
#endif
                {0x6000, OTYPE_RECORD, 96,    0, acName6000, SDO6000},
                {0x7000, OTYPE_RECORD, 96,    0, acName7000, SDO7000},
                {0x8000, OTYPE_RECORD, 1,    0, acName8000, SDO8000},
                {0xffff, 0xff,         0xff, 0xff, NULL, NULL}
        };







