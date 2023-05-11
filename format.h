#ifndef FORMAT
#define FORMAT
#include "type.h"

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#define NC_READ_U8(DATA) \
    ((uint8) *((uint8 *) (DATA)))

#define NC_READ_S8(DATA) \
     ((int8) *((uint8 *) (DATA)))

#define NC_READ_U16(DATA)  ((uint16) le16_to_cpup((void *) (DATA)))

#define NC_READ_S16(DATA) \
     ((int16) le16_to_cpup((void *) (DATA)))

#define                NC_READ_U32(DATA) \
     ((uint32) le32_to_cpup((void *) (DATA)))

#define NC_READ_S32(DATA) \
     ((int32) le32_to_cpup((void *) (DATA)))


#define NC_WRITE_S8(DATA, VAL) NC_WRITE_U8(DATA, VAL)

#define NC_WRITE_U8(DATA, VAL) \
    do { \
        *((uint8 *)(DATA)) = ((uint8) (VAL)); \
    } while (0)

#define NC_WRITE_U16(DATA, VAL) \
    do { \
        *((uint16 *) (DATA)) = cpu_to_le16((uint16) (VAL)); \
    } while (0)

#define NC_WRITE_S16(DATA, VAL) NC_WRITE_U16(DATA, VAL)

#define NC_WRITE_U32(DATA, VAL) \
    do { \
        *((uint32 *) (DATA)) = cpu_to_le32((uint32) (VAL)); \
    } while (0)

#define NC_WRITE_S32(DATA, VAL) NC_WRITE_U32(DATA, VAL)

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define le16_to_cpu(x) x
#define le32_to_cpu(x) x
#define le64_to_cpu(x) x

#define cpu_to_le16(x) x
#define cpu_to_le32(x) x
#define cpu_to_le64(x) x

#elif __BYTE_ORDER == __BIG_ENDIAN


#define swap16(x) \
        ((uint16)( \
        (((uint16)(x) & 0x00ffU) << 8) | \
        (((uint16)(x) & 0xff00U) >> 8) ))
#define swap32(x) \
        ((uint32)( \
        (((uint32)(x) & 0x000000ffUL) << 24) | \
        (((uint32)(x) & 0x0000ff00UL) <<  8) | \
        (((uint32)(x) & 0x00ff0000UL) >>  8) | \
        (((uint32)(x) & 0xff000000UL) >> 24) ))

#define le16_to_cpu(x) swap16(x)
#define le32_to_cpu(x) swap32(x)

#define cpu_to_le16(x) swap16(x)
#define cpu_to_le32(x) swap32(x)
#endif // FORMAT
#define le16_to_cpup(x) le16_to_cpu(*((uint16 *)(x)))
#define le32_to_cpup(x) le32_to_cpu(*((uint32 *)(x)))
/*#define le64_to_cpup(x) le64_to_cpu(*((uint64_t *)(x)))*/


uint16 nctohs(uint16 );
uint16 htoncs(uint16 );
uint32 nctohl(uint32 );
uint32 htoncl(uint32 );



#endif

