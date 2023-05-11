#ifndef OBJECT_H_
#define OBJECT_H_

//#include "Slave.h"
#include	"OD.h"

#ifndef NULL
#define NULL 0
#endif
#define FALSE false
#define TRUE true

// ------------------------------------------------------------------------------------------
// Device Infomartion
// ------------------------------------------------------------------------------------------
extern unsigned long   Obj_1000_d;
extern SubIndex_t      Obj_1000_s;
extern Index_t         Obj_1000;

// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_1029_n ;
extern unsigned char   Obj_1029_d[];
extern SubIndex_t      Obj_1029_s[] ;
extern Index_t         Obj_1029 ;

// ------------------------------------------------------------------------------------------
extern unsigned long   Obj_1006_d;
extern SubIndex_t      Obj_1006_s;
extern Index_t         Obj_1006;

// ------------------------------------------------------------------------------------------
extern char            Obj_1008_d[];
extern SubIndex_t      Obj_1008_s;
extern Index_t         Obj_1008;

// ------------------------------------------------------------------------------------------
extern char            Obj_1009_d[];
extern SubIndex_t      Obj_1009_s;
extern Index_t         Obj_1009;

// ------------------------------------------------------------------------------------------
extern char            Obj_100A_d[];
extern SubIndex_t      Obj_100A_s;
extern Index_t         Obj_100A;

// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_1018_n;
extern unsigned long   Obj_1018_d[];
extern SubIndex_t      Obj_1018_s[];
extern Index_t         Obj_1018;

// ------------------------------------------------------------------------------------------
// control word
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_6040_n ;     
extern unsigned long   Obj_6040_d ;
extern SubIndex_t      Obj_6040_s[] ;   
                                		
extern Index_t         Obj_6040 ;       

// ------------------------------------------------------------------------------------------
// status word
// ------------------------------------------------------------------------------------------
extern  unsigned char   Obj_6041_n;      
extern  unsigned long   Obj_6041_d;      
extern  SubIndex_t      Obj_6041_s[];    		
extern  Index_t         Obj_6041;       

// ------------------------------------------------------------------------------------------
// current position
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_6064_n;     
extern unsigned long   Obj_6064_d ;     
extern SubIndex_t      Obj_6064_s[];  		
extern Index_t         Obj_6064;       


// ------------------------------------------------------------------------------------------
// current velocity
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_606C_n;     
extern unsigned long   Obj_606C_d;      
extern SubIndex_t      Obj_606C_s[];   		
extern Index_t         Obj_606C;       

// ------------------------------------------------------------------------------------------
// target position
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_607A_n;      
extern unsigned long   Obj_607A_d;     
extern SubIndex_t      Obj_607A_s[];   			
extern Index_t         Obj_607A;       

// ------------------------------------------------------------------------------------------
// target velocity
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_60FF_n;      
extern unsigned long   Obj_60FF_d;      
extern SubIndex_t      Obj_60FF_s[];    			
extern Index_t         Obj_60FF;       


// ------------------------------------------------------------------------------------------
// Digital In PDOs
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_1a00_n;
extern unsigned long   Obj_1a00_d[];
extern SubIndex_t      Obj_1a00_s[];
extern Index_t         Obj_1a00;


// ------------------------------------------------------------------------------------------
// Digital Out PDOs
// ------------------------------------------------------------------------------------------
extern unsigned char   Obj_1600_n;
extern unsigned long   Obj_1600_d[];
extern SubIndex_t      Obj_1600_s[];
extern Index_t         Obj_1600;



// -----------------------------------------------------------------------------------------------------------
// Rx PDO Assigns (Mater->Slave) 4*2Byte   
// -----------------------------------------------------------------------------------------------------------
extern unsigned char   Obj_1C00_n;      
extern unsigned short  Obj_1C00_d[];   
extern SubIndex_t      Obj_1C00_s[];    
extern Index_t         Obj_1C00;       

// -----------------------------------------------------------------------------------------------------------
// Tx PDO Assigns (Slave->Mater) 2*2Byte   
// -----------------------------------------------------------------------------------------------------------
extern unsigned char   Obj_1C01_n;    
extern unsigned short  Obj_1C01_d[];   
extern SubIndex_t      Obj_1C01_s[];  
                               
extern Index_t         Obj_1C01;   



// ------------------------------------------------------------------------------------------
// Device Infomartion
// ------------------------------------------------------------------------------------------
extern unsigned long   Obj_67FF_d ;
extern SubIndex_t      Obj_67FF_s;
extern Index_t         Obj_67FF;

// -----------------------------------------------------------------------------------
// Object Dictionary
// -----------------------------------------------------------------------------------
extern Index_t *       pObject[];

extern unsigned short      ObjectNum;




#endif

