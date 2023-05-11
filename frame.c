#include "frame.h"
#include "string.h"
#include "format.h"

#include "datagram.h"
#include "bufferSendrecv.h"
//#include "dldriver.h"
#define MINIMUM_FRAME_SIZE 60

static mac_addr_t destination = {0x11,0x11,0x11,0x11,0x11,0x11};
static mac_addr_t source = {0x22,0x22,0x22,0x22,0x22,0x22};
static uint16 frameType = 0x8888;

static uint8 FrameIndex = 0x00;     //帧索引
const uint8 FrameVersion = 0x02;    //帧版本

static void initEthernethdr(ethernet_hdr_t *ethdr)
{
    memcpy(ethdr->dest,destination,sizeof(mac_addr_t));
    memcpy(ethdr->src,source,sizeof(mac_addr_t));
    ethdr->type = frameType;
    return;
}

void initframe(nc_frame_t *frm)
{
    if(!frm)
        return;
    memset(frm,0,sizeof(nc_frame_t));
    initEthernethdr(&(frm->ethdr));             /*设置mac地址*/
    frm->index = FrameIndex;                    /*设置frameindex*/
    frm->frameVersion = FrameVersion;           /*设置版本号，cycle和circulation flag都为0*/
    memset(frm->dgrambuff,0,sizeof(frm->dgrambuff));    /*frame数据域区初始化，用于填充datagrams*/
    frm->currentdataptr = frm->dgrambuff;
    frm->lastcmd = NULL;                        /*当前无报文，无上一条命令*/
}


int FrameTransmit(const nc_frame_t *frm)
{
    uint8 buff[1600];
    memset(buff,0x00,1600);
    uint8 *ptr = buff;
    if(!frm || frm->currentdataptr == frm->dgrambuff)
        return -1;

    uint16 frameLen;

    memcpy(ptr,&(frm->ethdr.dest),sizeof(mac_addr_t));
    ptr += sizeof(mac_addr_t);
    memcpy(ptr,&(frm->ethdr.src),sizeof(mac_addr_t));
    ptr += sizeof(mac_addr_t);
    *(uint16*)ptr = htoncs(frm->ethdr.type);
    ptr += 2;

    *ptr++ = frm->index;
    *ptr++ = frm->frameVersion;

    uint16 datagramLen = (frm->currentdataptr - frm->dgrambuff)/sizeof(uint8);
    memcpy(ptr,frm->dgrambuff,datagramLen);
    ptr += datagramLen;

    frameLen = (ptr - buff)/sizeof(uint8);



#if 0

    if(frameLen<MINIMUM_FRAME_SIZE)
    {
        memset(ptr,0,MINIMUM_FRAME_SIZE-frameLen);
        frameLen=MINIMUM_FRAME_SIZE;
    }
    int i = 0;
    do
    {
        printf("%02x ",*(buff + i));
        i++;
    }while(i < frameLen);
    printf("\n");
    return 0;

#else

 if(frameLen < MINIMUM_FRAME_SIZE)
    {
        if(send_buffer(buff,MINIMUM_FRAME_SIZE) != MINIMUM_FRAME_SIZE)
            return -1;
        return MINIMUM_FRAME_SIZE;
    }
    else
    {
        if(send_buffer(buff,frameLen) != frameLen)
            return -1;
        return frameLen;
    }

#endif


}


/***此函数必须传进一个nc_frame_t的实例指针***/

int FrameReceive(nc_frame_t *recvfrm)
{
    /******帧索引，每收到一帧就加1********/
    FrameIndex++;
#if 1
    uint8 buff[1600];

    uint8 *ptr = buff;
    

    int n = rev_buffer(buff,1000);
    
    while (n ==-1)
    {
        FrameTransmit(recvfrm);
        n = rev_buffer(buff,1000);
    }
    
    initframe(recvfrm);

    memcpy(recvfrm->ethdr.dest,ptr,sizeof(mac_addr_t));
    ptr += sizeof(mac_addr_t);
   
    memcpy(recvfrm->ethdr.src,ptr,sizeof(mac_addr_t));
    ptr += sizeof(mac_addr_t);

    recvfrm->ethdr.type = nctohs(*(uint16*)ptr);
    ptr += sizeof(uint16);

    recvfrm->index = *ptr;
    ptr += sizeof(uint8);

    recvfrm->frameVersion = *ptr;
    ptr += sizeof(uint8);
    memcpy(recvfrm->dgrambuff,ptr,n - 16);
    //recvfrm->currentdataptr = recvfrm->dgrambuff + (n - 16);
    uint16 dataLen;
    while((*ptr) & 0x80){
        switch((*ptr) & 0x7F)
        {
            case APRD:
            case APWR:
            case FPRD:
            case FPWR:
            case BRD:
            case BWR:

            case ACHRD:
            case ACHWR:
            case FCHRD:
            case FCHWR:
                ptr += 5;
                dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
                ptr += sizeof(uint16) + dataLen + 2;
                break;
            case SINGLECYCLEMAP:
            case DOUBLECYCLEMAP:
                ptr += 3;
                dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
                ptr += sizeof(uint16) + dataLen + 4;
                break;
            case TSCMD:
                ptr += 1;
                break;
            case SYNCRESETCMD:
            case SYNCCOMMAND:
                ptr += 4;
                break;
            case DC32COMMAND:
                ptr += 5;
                break;
            case DC64COMMAND:
                 ptr += 9;
        }
    }
    recvfrm->lastcmd = ptr;

    switch((*ptr) & 0x7F)
    {
        case APRD:
        case APWR:
        case FPRD:
        case FPWR:
        case BRD:
        case BWR:

        case ACHRD:
        case ACHWR:
        case FCHRD:
        case FCHWR:
            ptr += 5;
            dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
            ptr += sizeof(uint16) + dataLen + 2;
            break;
        case SINGLECYCLEMAP:
        case DOUBLECYCLEMAP:
            ptr += 3;
            dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
            ptr += sizeof(uint16) + dataLen + 4;
            break;
        case TSCMD:
            ptr += 1;
            break;
        case SYNCRESETCMD:
        case SYNCCOMMAND:
            ptr += 4;
            break;
        case DC32COMMAND:
            ptr += 5;
            break;
        case DC64COMMAND:
             ptr += 9;
    }
    recvfrm->currentdataptr = ptr;

    uint16 irq;
     irq = nctohs(*(uint16 *)(recvfrm->dgrambuff + 8));

    return n;
#endif
}


void dest_mac_setting(mac_addr_t dest)
{
    memcpy(destination,dest,sizeof(mac_addr_t));
    return;
}

void src_mac_setting(mac_addr_t src)
{
    memcpy(source,src,sizeof(mac_addr_t));
    return;
}

