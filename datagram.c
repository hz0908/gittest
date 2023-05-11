#include "format.h"
#include "string.h"
#include "stdio.h"
#include "datagram.h"

#define MAX_DGRAM_LEN 1498

/**返回插入报文距离第一个报文起始地址的长度**/
int16 regAccessdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 st,
                       uint16 offset,uint16 datalen,uint8* data,uint16 irq)
{
    /*检测该帧是否有足够的空间插入新的报文*/
    if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                         + datalen + 9 > MAX_DGRAM_LEN)
        return -1;

    switch(cmd)
    {
        case APRD:
        case APWR:
                st = ~st + 1;       /*顺序地址，取反+1，每过一个从站该数据域+1，为0时，寻址命中*/
                break;
        case FPRD:
        case FPWR:
                break;
        case BRD:
        case BWR:
                st = 0x0000;
                break;
        default:
            return -1;
    }

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = cmd;         /**命令码**/

    *(uint16*)(targetFrame->currentdataptr) = htoncs(st);  /**st**/
    targetFrame->currentdataptr += sizeof(st);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(offset);   /**offset**/
    targetFrame->currentdataptr += sizeof(offset);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(datalen);   /**datalen**/
    targetFrame->currentdataptr += sizeof(datalen);

    memcpy(targetFrame->currentdataptr,data,datalen);       /**data field**/
    targetFrame->currentdataptr += datalen;

    *(uint16*)(targetFrame->currentdataptr) = htoncs(irq);   /**irq**/
    targetFrame->currentdataptr += sizeof(irq);

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);
}


int16 chAccessdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 st,\
                            uint16 offset,uint16 datalen,uint8 ch,uint8* data,uint16 irq)
{
    /*检测该帧是否有足够的空间插入新的报文*/
    if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                         + datalen + 9 > MAX_DGRAM_LEN)
        return -1;

    switch(cmd)
    {
        case ACHRD:
        case ACHWR:
                st = ~st + 1;       /*顺序地址，取反+1，每过一个从站该数据域+1，为0时，寻址命中*/
                break;
        case FCHRD:
        case FCHWR:
                break;
        default:
            return -1;
    }

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = cmd;         /**命令码**/

    *(uint16*)(targetFrame->currentdataptr) = htoncs(st);  /**st**/
    targetFrame->currentdataptr += sizeof(st);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(offset);   /**offset**/
    targetFrame->currentdataptr += sizeof(offset);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(datalen | ch<<11);   /**datalen**/
    targetFrame->currentdataptr += sizeof(datalen);

    memcpy(targetFrame->currentdataptr,data,datalen);       /**data field**/
    targetFrame->currentdataptr += datalen;

    *(uint16*)(targetFrame->currentdataptr) = htoncs(irq);   /**irq**/
    targetFrame->currentdataptr += sizeof(irq);

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);
}

int16 chMapdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 mapNo,uint16 datalen,\
                                                        uint8 *data,uint16 irq,uint16 mfmo)
{
    /*检测该帧是否有足够的空间插入新的报文*/
    if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                         + datalen + 9 > MAX_DGRAM_LEN)
        return -1;

    switch(cmd)
    {
        case SINGLECYCLEMAP:
        case DOUBLECYCLEMAP:
                break;
        default:
            return -1;
    }

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = cmd;         /**命令码**/

    *(uint16*)(targetFrame->currentdataptr) = htoncs(mapNo);  /**mapNo**/
    targetFrame->currentdataptr += sizeof(mapNo);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(datalen);   /**datalen**/
    targetFrame->currentdataptr += sizeof(datalen);

    memcpy(targetFrame->currentdataptr,data,datalen);       /**data field**/
    targetFrame->currentdataptr += datalen;

    *(uint16*)(targetFrame->currentdataptr) = htoncs(irq);   /**irq**/
    targetFrame->currentdataptr += sizeof(irq);

    *(uint16*)(targetFrame->currentdataptr) = htoncs(mfmo);   /**mfmo**/
    targetFrame->currentdataptr += sizeof(mfmo);

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);

}

int16 timesdgrmAdd(nc_frame_t *targetFrame)
{
    if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                         + 1 > MAX_DGRAM_LEN)
        return -1;

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = TSCMD;         /**命令码**/

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);
}


int16 syncdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint8* data)
{
    if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                         + 4 > MAX_DGRAM_LEN)
        return -1;

    switch(cmd)
    {
        case SYNCRESETCMD:
        case SYNCCOMMAND:
                break;
        default:
            return -1;
    }

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = cmd;         /**命令码**/

    memcpy(targetFrame->currentdataptr,data,3);       /**data field**/
    targetFrame->currentdataptr += 3;                 /**同步/同步复位报文3字节长度**/

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);
}

int16 dclkdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint8* data)
{
    uint8 space[8] = {0};

    switch(cmd)
    {
        case DC32COMMAND:
            if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                                 + 5 > MAX_DGRAM_LEN)
                return -1;
            break;

        case DC64COMMAND:
            if((targetFrame->currentdataptr - targetFrame->dgrambuff)/sizeof(uint8) \
                                                                 + 9 > MAX_DGRAM_LEN)
                return -1;
            break;
        default:
            return -1;
    }

    /**插入报文**/
    /**当前插入报文不是是第一个报文，将上一帧的M位置1**/
    if(targetFrame->lastcmd)
        *(targetFrame->lastcmd)|= 0x80;

    targetFrame->lastcmd = targetFrame->currentdataptr;

    *(targetFrame->currentdataptr)++ = cmd;         /**命令码**/

    switch(cmd)
    {
        case DC32COMMAND:
            if(data)
                memcpy(targetFrame->currentdataptr,data,4);       /**data field**/
            else
                memcpy(targetFrame->currentdataptr,space,4);
            targetFrame->currentdataptr += 4;                 /**32位分布时钟4字节**/
            break;

        case DC64COMMAND:
            if(data)
                memcpy(targetFrame->currentdataptr,data,8);       /**data field**/
            else
                memcpy(targetFrame->currentdataptr,space,8);
            targetFrame->currentdataptr += 8;                 /**64位分布时钟8字节**/
            break;
    }

    return (targetFrame->lastcmd - targetFrame->dgrambuff)/sizeof(uint8);
}

/***用于验证接收到的datagram是否为一个合格的报文(检查IRQ的内容)，并返回报文长度***/
/***传入的dgrm_start_byte为报文的起始字节,bool flag表示该帧正确或错误***/
//static int16 verifrydgrm(const uint8* dgrm_start_byte,bool *flag)
//{
//    uint8* ptr = dgrm_start_byte;
//    uint16 dataLen,irq,mfmo;
//    switch((*dgrm_start_byte) & 0x7F)
//    {
//        case APRD:
//        case APWR:
//        case FPRD:
//        case FPWR:
//        case BRD:
//        case BWR:

//        case ACHRD:
//        case ACHWR:
//        case FCHRD:
//        case FCHWR:
//            ptr += 5;
//            dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
//            ptr += sizeof(uint16) + dataLen;

//            irq = nctohs(*(uint16*)ptr);
//            if(irq & 0x03 == 0x01)
//                *flag = true;
//            else
//                *flag = false;
//            ptr += sizeof(uint16);
//            break;
//        case SINGLECYCLEMAP:
//        case DOUBLECYCLEMAP:
//            ptr += 3;
//            dataLen = nctohs(*(uint16*)ptr) & 0x07FF;
//            ptr += sizeof(uint16) + dataLen;
//            /**跳过IRQ检验,IRQ长度2字节**/
//            ptr += sizeof(uint16);
//            mfmo = nctohs(*((uint16*)ptr));
//            if(mfmo & 0xFF00 == 0x0000)
//                *flag = true;
//            else
//                *flag = false;
//            ptr += sizeof(uint16);
//            break;
//        case TSCMD:
//            *flag = true;
//            ptr += sizeof(uint8);
//            break;
//        case SYNCRESETCMD:
//        case SYNCCOMMAND:
//            *flag = true;
//            ptr += 4;
//            break;
//        case DC32COMMAND:
//            *flag = true;
//            ptr += 5;
//            break;
//        case DC64COMMAND
//            *flag = true;
//            ptr += 9;
//            break;
//        default:
//            *flag = false;
//            ptr += 1;
//    }

//    /*****返回检验的单帧的总长度*******/
//    return (ptr - dgrm_start_byte)/sizeof(uint8);
//}

//bool datagrmsIsErr(const uint8 *datagrams)
//{
//    uint8 *cmd_ptr = datagrams;
//    bool Flag;
//    uint16 len;

//    while(*cmd_ptr & 0x80)
//    {
//        len = verifrydgrm(cmd_ptr,&Flag);
//        if(!Flag)   /**有一个datagram检测到读写不正确**/
//            return false;
//        cmd_ptr += len;
//    }
//    len = verifrydgrm(cmd_ptr,&Flag);
//    if(!Flag)   /**有一个datagram检测到读写不正确**/
//        return false;

//}


