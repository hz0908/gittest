#ifndef OD_H_
#define OD_H_


// ========================================================================================================
//    Static Data Types
// ========================================================================================================

#define	DEFTYPE_NULL				0x0000
#define	DEFTYPE_BOOLEAN 		         0x0001
#define	DEFTYPE_INTEGER8			0x0002
#define	DEFTYPE_INTEGER16			0x0003
#define	DEFTYPE_INTEGER32			0x0004
#define	DEFTYPE_UNSIGNED8			0x0005
#define	DEFTYPE_UNSIGNED16		         0x0006
#define	DEFTYPE_UNSIGNED32		         0x0007
#define	DEFTYPE_REAL32				0x0008
#define	DEFTYPE_VISIBLESTRING	         0x0009
#define	DEFTYPE_OCTETSTRING		         0x000A
#define	DEFTYPE_UNICODESTRING		0x000B
#define	DEFTYPE_TIMEOFDAY		         0x000C
#define	DEFTYPE_TIMEDIFFERENCE		0x000D
#define	DEFTYPE_DOMAIN	                            0x000F
#define	DEFTYPE_INTEGER24		         0x0010  
#define	DEFTYPE_REAL64	                           0x0011
#define	DEFTYPE_INTEGER40		0x0012
#define	DEFTYPE_INTEGER48		0x0013
#define	DEFTYPE_INTEGER56		0x0014
#define	DEFTYPE_INTEGER64		0x0015
#define	DEFTYPE_UNSIGNED24		0x0016
#define	DEFTYPE_UNSIGNED40		0x0018
#define	DEFTYPE_UNSIGNED48		0x0019
#define	DEFTYPE_UNSIGNED56		0x001A
#define	DEFTYPE_UNSIGNED64		0x001B

#define	DEFTYPE_PDOCOMMUNICATIONPARMETER				0x0020
#define	DEFTYPE_PDOMAPPING		         0x0021
#define	DEFTYPE_SDOPARAMETER		0x0022
#define	DEFTYPE_IDENTITY			0x0023



#define OBJCODE_VAR 				0x07
#define OBJCODE_ARR 				0x08
#define OBJCODE_REC 				0x09



// ========================================================================================================
//    Flag
// ========================================================================================================

#define     RN          0x00    // Readonly,    PdoNo
#define     WN          0x10    // ReadWrite,   PdoNo
#define     RP          0x01    // Readonly,    PdoYes
#define     WP          0x11    // ReadWrite,   PdoYes

// ========================================================================================================
// Object Define
// ========================================================================================================
typedef struct
{
    unsigned short      DataType;

    unsigned char       AccessFlag;
    unsigned char       ObjectBytes;
    void                *pData;
    char                *sName;

}   SubIndex_t;

typedef struct
{
    unsigned  char      ObjectCode;

    unsigned short      Index;
    unsigned char       MaxSubindex;
    unsigned char       SubindexNum;
    SubIndex_t          *pSubIndex;

}   Index_t;

// ========================================================================================================
// Object Functions
// ========================================================================================================

Index_t *  OdScanIndex(unsigned short Index);
int        OdReadSubIndex(void *pBuf, unsigned short Index, unsigned char SubIndex);
int        OdWriteSubIndex(const void *pBuf, unsigned short Index, unsigned char SubIndex);

#endif // OD_H_

