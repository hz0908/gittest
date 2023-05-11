#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame.h"
#include "format.h"
#include "string.h"
#include "register.h"
#include "mapping_visit.h"
#include "datagram.h"
#include "bufferSendrecv.h"
#include "mapping_config.h"

//打印函数
void printData(const uint8* data, uint16 len) 
{
    for (uint16 i = 0; i < len; ++i) 
    {
    printf("%02X ", data[i]); // 以16进制格式打印每个字节的值
    }
printf("\n");
}

//通道映射读函数
bool mapping_singlecycle_rd(uint16 MAPNo, uint16 len, uint8* data)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;
    /**清空读取的缓存区**/
    memset(data,0,len);
    initframe(&frame);
    /***站内偏移地址设为0x00,irq设为0x00***/
    result = chMapdgrmAdd(&frame, SINGLECYCLEMAP, MAPNo, len, data, 0x00, 0x00);
    /***报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度***/
    if(result < 0)
        return false;

    FrameTransmit(&frame);
    if(FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16*)(frame.dgrambuff + 5 +len));
    if((irq & 0x03) != 0x01)
        return false;

    //将读取到的数据存入data中
    memcpy(data,frame.dgrambuff + 5,len);
    printf("Data: ");
    printData(data, len); // 调用printData函数打印data中的数据
    return true;
}

//通道映射写函数
bool mapping_singlecycle_wr(uint16 MAPNo, uint16 len, uint8* data)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;
    // /**清空读取的缓存区**/
    // memset(data,0,len);
    initframe(&frame);
    /***站内偏移地址设为0x00,irq设为0x00***/
    result = chMapdgrmAdd(&frame, SINGLECYCLEMAP, MAPNo, len, data, 0x00, 0x00);
    /***报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度***/
    if(result < 0)
        return false;

    FrameTransmit(&frame);
    if(FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16*)(frame.dgrambuff + 5 +len));
    if((irq & 0x03) != 0x01)
        return false;

    // //将读取到的数据存入data中
    // memcpy(data,frame.dgrambuff + 5,len);
    // printf("Data: ");
    // printData(data, len); // 调用printData函数打印data中的数据
    return true;
}

