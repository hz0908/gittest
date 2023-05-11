#include <stdlib.h>
#include <string.h>

#include "OD.h"
#include "Object.h"

// ========================================================================================================
// Object Functions
// ========================================================================================================
Index_t *   OdScanIndex(unsigned short Index)
{
	  unsigned short  i;
	
    static  unsigned short  LastIndex   = 0;
    static  Index_t *       LastPointer = NULL;
    if(Index==LastIndex)    return LastPointer;

    for(i=0;i<ObjectNum;i++)
    {
        if(pObject[i]->Index == Index)
        {
            LastIndex   = Index;
            LastPointer = pObject[i];
            return pObject[i];
        }
    }
    return  NULL;
}

int        OdReadSubIndex(void *pBuf, unsigned short Index, unsigned char SubIndex)
{
	  SubIndex_t *    pSubIndex;
    Index_t *       pIndex = OdScanIndex(Index);
    if(pIndex==NULL) return -1;

    if(SubIndex >= pIndex->SubindexNum) return -2;

    pSubIndex = pIndex->pSubIndex + SubIndex;
    memcpy(pBuf,pSubIndex->pData,pSubIndex->ObjectBytes);

    return  0;
}

int        OdWriteSubIndex(const void *pBuf, unsigned short Index, unsigned char SubIndex)
{
	  SubIndex_t *    pSubIndex;
    Index_t *       pIndex = OdScanIndex(Index);
    if(pIndex==NULL) return -1;

    if(SubIndex >= pIndex->SubindexNum) return -2;

    pSubIndex = pIndex->pSubIndex + SubIndex;
    if((pSubIndex->AccessFlag & WN)==0) return -3;

    memcpy(pSubIndex->pData,pBuf,pSubIndex->ObjectBytes);

    return  0;
}





