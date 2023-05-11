#include  "Object.h"
#include	"OD.h"

// ------------------------------------------------------------------------------------------
// Device Infomartion
// ------------------------------------------------------------------------------------------
unsigned long   Obj_1000_d      = 0x000f0191;
SubIndex_t      Obj_1000_s      = {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1000_d, "Device Type"};
Index_t         Obj_1000        = {OBJCODE_VAR,0x1000, 1, 1, &Obj_1000_s};

// ------------------------------------------------------------------------------------------


unsigned char   Obj_1029_n      = 3;
unsigned char   Obj_1029_d[]    = {0x00, 0x00, 0x00};
SubIndex_t      Obj_1029_s[]    = {{DEFTYPE_UNSIGNED8, RN, 1, &Obj_1029_n,     "error behaviour"},
                                   {DEFTYPE_UNSIGNED8, RN, 1, &Obj_1029_d[0],  "communication error"},
                                   {DEFTYPE_UNSIGNED8, RN, 1, &Obj_1029_d[1],  "output error"},
                                   {DEFTYPE_UNSIGNED8, RN, 1, &Obj_1029_d[3],  "input error"}};
Index_t         Obj_1029        = {OBJCODE_VAR,0x1029, 4, 4, Obj_1029_s};

// ------------------------------------------------------------------------------------------
unsigned long   Obj_1006_d      = 0x00000000;
SubIndex_t      Obj_1006_s      = {DEFTYPE_UNSIGNED32, WN, 4, &Obj_1006_d, "Communication Cycle Period"};
Index_t         Obj_1006        = {OBJCODE_VAR,0x1006, 1, 1, &Obj_1006_s};

// ------------------------------------------------------------------------------------------
char            Obj_1008_d[]    = "EtherCat DS401 IO Device";
SubIndex_t      Obj_1008_s      = {DEFTYPE_UNSIGNED32, RN, sizeof(Obj_1008_d), Obj_1008_d, "Deivce Name"};
Index_t         Obj_1008        = {OBJCODE_VAR,0x1008, 1, 1, &Obj_1008_s};

// ------------------------------------------------------------------------------------------
char            Obj_1009_d[]    = "1.00";
SubIndex_t      Obj_1009_s      = {DEFTYPE_UNSIGNED32, RN, sizeof(Obj_1009_d), Obj_1009_d, "Hardware version"};
Index_t         Obj_1009        = {OBJCODE_VAR,0x1009, 1, 1, &Obj_1009_s};

// ------------------------------------------------------------------------------------------
char            Obj_100A_d[]    = "1.00";
SubIndex_t      Obj_100A_s      = {DEFTYPE_UNSIGNED32, RN, sizeof(Obj_100A_d), Obj_100A_d, "Software version"};
Index_t         Obj_100A        = {OBJCODE_VAR,0x100A, 1, 1, &Obj_100A_s};

// ------------------------------------------------------------------------------------------
unsigned char   Obj_1018_n      = 4;
unsigned long   Obj_1018_d[]    = {0x80000000, 0xD4050401, 0x00010000, 0x00000000};
SubIndex_t      Obj_1018_s[]    = {{DEFTYPE_UNSIGNED8, RN, 1, &Obj_1018_n,     "Identity"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1018_d[0],  "Vendor ID"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1018_d[1],  "Product Code"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1018_d[2],  "Revision"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1018_d[3],  "Serial Number"}};
Index_t         Obj_1018        = {OBJCODE_VAR,0x1018, 5, 5, Obj_1018_s};


// ------------------------------------------------------------------------------------------
// control word
// ------------------------------------------------------------------------------------------
unsigned char   Obj_6040_n      = 1;
unsigned long   Obj_6040_d      = 1;
SubIndex_t      Obj_6040_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_6040_n,     "control word"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_6040_d,  "control word"}};			
Index_t         Obj_6040        = {OBJCODE_ARR, 0x6040, 2, 2, Obj_6040_s};

// ------------------------------------------------------------------------------------------
// status word
// ------------------------------------------------------------------------------------------
unsigned char   Obj_6041_n      = 1;
unsigned long   Obj_6041_d      = 10;
SubIndex_t      Obj_6041_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_6041_n,     "status word"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_6041_d,  "status word"}};			
Index_t         Obj_6041        = {OBJCODE_ARR, 0x6041, 2, 2, Obj_6041_s};

// ------------------------------------------------------------------------------------------
// current position
// ------------------------------------------------------------------------------------------
unsigned char   Obj_6064_n      = 1;
unsigned long   Obj_6064_d      = 0;
SubIndex_t      Obj_6064_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_6064_n,     "current position"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_6064_d,  "current position"}};			
Index_t         Obj_6064        = {OBJCODE_ARR, 0x6064, 2, 2, Obj_6064_s};


// ------------------------------------------------------------------------------------------
// current velocity
// ------------------------------------------------------------------------------------------
unsigned char   Obj_606C_n      = 1;
unsigned long   Obj_606C_d      = 0;
SubIndex_t      Obj_606C_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_606C_n,     "current velocity"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_606C_d,  "current velocity"}};			
Index_t         Obj_606C        = {OBJCODE_ARR, 0x606C, 2, 2, Obj_606C_s};

// ------------------------------------------------------------------------------------------
// target position
// ------------------------------------------------------------------------------------------
unsigned char   Obj_607A_n      = 1;
unsigned long   Obj_607A_d      = 0;
SubIndex_t      Obj_607A_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_607A_n,     "target position"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_607A_d,  "target position"}};			
Index_t         Obj_607A        = {OBJCODE_ARR, 0x607A, 2, 2, Obj_607A_s};


// ------------------------------------------------------------------------------------------
// target velocity
// ------------------------------------------------------------------------------------------
unsigned char   Obj_60FF_n      = 1;
unsigned long   Obj_60FF_d      = 0;
SubIndex_t      Obj_60FF_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_60FF_n,     "target velocity"},
                                   {DEFTYPE_UNSIGNED32, WP, 4, &Obj_60FF_d,  "target velocity"}};			
Index_t         Obj_60FF        = {OBJCODE_ARR, 0x60FF, 2, 2, Obj_60FF_s};



// ------------------------------------------------------------------------------------------
// rx PDOs
// ------------------------------------------------------------------------------------------
unsigned char   Obj_1600_n      = 3;
unsigned long   Obj_1600_d[]    = {0x60400120,0x607A0120,0x60FF0120};
SubIndex_t      Obj_1600_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_1600_n,  "Driver command"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1600_d[0],  "Driver command control_word"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1600_d[1],  "Driver command target position"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1600_d[2],  "Driver command target velocity"}};

Index_t         Obj_1600        = {OBJCODE_ARR, 0x1600, 4, 4, Obj_1600_s};


// ------------------------------------------------------------------------------------------
// Analog In PDOs     ==      Analog Output PDOs    ???????????????
// ------------------------------------------------------------------------------------------

unsigned char   Obj_1a00_n      = 3;
unsigned long   Obj_1a00_d[]    = {0x60410120,0x60640120,0x606C0120};
SubIndex_t      Obj_1a00_s[]    = {{DEFTYPE_UNSIGNED8,RN, 1, &Obj_1a00_n,  "Driver information"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1a00_d[0],  "Driver information status_word"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1a00_d[1],  "Driver information current_position"},
                                   {DEFTYPE_UNSIGNED32, RN, 4, &Obj_1a00_d[2],  "Driver information current velocity"}};
                                  
Index_t         Obj_1a00        = {OBJCODE_ARR, 0x1a00, 4, 4, Obj_1a00_s};



// -----------------------------------------------------------------------------------------------------------
// Rx PDO Assigns (Mater->Slave) 4*2Byte   
// -----------------------------------------------------------------------------------------------------------
unsigned char   Obj_1C00_n      = 1;
unsigned short  Obj_1C00_d[]    = {0x1600};
SubIndex_t      Obj_1C00_s[]    = {{DEFTYPE_UNSIGNED8,WN, 1, &Obj_1C00_n,     "Rx Pdo1 Assigns (Mater->Slave)"},
                                   {DEFTYPE_UNSIGNED16, WN, 2, &Obj_1C00_d[0],  "SubIndex 1"}};
Index_t         Obj_1C00        = {OBJCODE_ARR, 0x1C00, 2, 2, Obj_1C00_s};


// -----------------------------------------------------------------------------------------------------------
// Tx PDO Assigns (Slave->Mater) 2*2Byte   
// -----------------------------------------------------------------------------------------------------------
unsigned char   Obj_1C01_n      = 1;
unsigned short  Obj_1C01_d[]    = {0x1a00};
SubIndex_t      Obj_1C01_s[]    = {{DEFTYPE_UNSIGNED8,WN, 1, &Obj_1C01_n,     "Tx PDO Assigns (Slave->Mater)"},
                                   {DEFTYPE_UNSIGNED16, WN, 2, &Obj_1C01_d[0],  "SubIndex 1"}};
Index_t         Obj_1C01        = {OBJCODE_REC, 0x1C01, 2, 2, Obj_1C01_s};




// ------------------------------------------------------------------------------------------
// Device Infomartion
// ------------------------------------------------------------------------------------------
unsigned long   Obj_67FF_d      = 0x000f0191;
SubIndex_t      Obj_67FF_s      = {DEFTYPE_UNSIGNED32, RN, 4, &Obj_67FF_d, "Device Type"};
Index_t         Obj_67FF        = {OBJCODE_VAR, 0x67FF, 1, 1, &Obj_67FF_s};


// -----------------------------------------------------------------------------------
// Object Dictionary
// -----------------------------------------------------------------------------------
Index_t *       pObject[] =
{
                &Obj_1000,
                &Obj_1029,
                &Obj_1006,
                &Obj_1008,
                &Obj_1009,
                &Obj_100A,
                &Obj_1018,
                &Obj_6040,
                &Obj_6041,
                &Obj_6064,
                &Obj_606C,
                &Obj_607A,
                &Obj_60FF,
                &Obj_1600,
                &Obj_1a00,
                &Obj_1C00,
                &Obj_1C01,
                &Obj_67FF,

};

unsigned short      ObjectNum       = sizeof(pObject) / sizeof(Index_t *);


