#ifndef FRAME
#define FRAME
#include "type.h"
#include "stdio.h"

typedef uint8 mac_addr_t[6];

/*以太网帧头*/
typedef struct ethernet_hdr ethernet_hdr_t;
struct ethernet_hdr
{
    mac_addr_t dest;            /*目的mac地址*/
    mac_addr_t src;             /*源mac地址*/
    uint16 type;                /*NCUC默认0x8888*/
};

typedef struct nc_frame nc_frame_t;
struct nc_frame
{
    ethernet_hdr_t ethdr;
    uint8 index;
    uint8 frameVersion;
    uint8 dgrambuff[1600];
    uint8* currentdataptr; /**当前填充，指向的指向dgrambuff的指针**/
    uint8* lastcmd;         /**上一个报文的cmd指针，在插入一个报文后，将上一个报文的cmd高位置1，并指向插入当前报文的cmd**/
};


void initframe(nc_frame_t *frm);
int FrameTransmit(const nc_frame_t *frm);
int FrameReceive(nc_frame_t *recvfrm);
void dest_mac_setting(mac_addr_t dest);
void src_mac_setting(mac_addr_t src);
#endif // FRAME

