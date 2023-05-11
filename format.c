#include "format.h"


/** NCUC2.0 frame transmission format similar with Little Endian **/
/** NCUC format to host format  For uint16 **/

uint16 nctohs(uint16 nc_format)
{
#ifdef BIG_ENDIAN

    uint16 h_format
    h_format = ((nc_format<<8) | (nc_format>>8));
    return h_format;

#else

    return nc_format;

#endif
}

/** host format to NCUC For uint16 **/
uint16 htoncs(uint16 h_format)
{
#ifdef BIG_ENDIAN

    uint16 nc_format
    nc_format = ((h_format<<8) | (h_format>>8));
    return nc_format;

#else

        return h_format;

#endif
}

/** NCUC format to host format  For uint32 **/
uint32 nctohl(uint32 nc_format)
{
#ifdef BIG_ENDIAN

    uint32 h_format = 0x00000000;
    h_format |= ((nc_format & 0x000000FF) << 24) |
        ((nc_format & 0x0000FF00) << 8) |
            ((nc_format & 0x00FF0000) >> 8) |
                ((nc_format & 0xFF000000) >> 24);
    return h_format;

#else

    return nc_format;

#endif
}

/** host format to NCUC format For uint32 **/

uint32 htoncl(uint32 h_format)
{
#ifdef BIG_ENDIAN

    uint32 h_format = 0x00000000;
    h_format |= ((nc_format & 0x000000FF) << 24) | ((nc_format & 0x0000FF00) << 8) | ((nc_format & 0x00FF0000) >> 8) | ((nc_format & 0xFF000000) >> 24);
    return h_format;

#else

    return h_format;

#endif
}
