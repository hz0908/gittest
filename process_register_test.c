#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include <sys/time.h>
#include <unistd.h>

//测试变量区
char send_card_name[]="enp1s0";//命令行ifconfig -a 查看
char rev_card_name[]="enp2s0";//ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文

uint16 st=0;
uint16 addr=0x00;
uint16 datalen=0x04;
uint8 data=0x12;
uint16 irq=0;
bool valid=true;


// 测试程序
int main()
{
//套接字创建 
send_socketCreate(send_card_name);
rev_socketCreate(rev_card_name);


//帧初始化
nc_frame_t frm;
initframe(&frm);

//报文添加
//regAccessdgrmAdd(&frm,APRD,st,addr,datalen,data,irq);


//报文发送测试
//FrameTransmit(&frm);
uint8 uint8_data=0;
uint16 uint16_data=0;
uint32 uint32_data=0;
uint8 re_uint8_data=0;
uint16 re_uint16_data=0;
uint32 re_uint32_data=0;
int count =1;


for (size_t i = 0; i < count; i++)
{
    uint8_data=nc_slave_apwr_uint8(st,0x126,uint8_data,&valid);
    printf("uint8_data write:%2x\n",uint8_data);
    // re_uint8_data=nc_slave_aprd_uint8(st,0x60,&valid);
    // printf("uint8_data read:%2x\n\n",re_uint8_data);
    // uint8_data++;
        uint8_data=nc_slave_apwr_uint8(st,0x12c,uint8_data,&valid);
    printf("uint8_data write:%2x\n",uint8_data);

        uint8_data=nc_slave_apwr_uint8(st,0x12d,uint8_data,&valid);
    printf("uint8_data write:%2x\n",uint8_data);

    uint8_data=nc_slave_apwr_uint8(st,0x12e,uint8_data,&valid);
    printf("uint8_data write:%2x\n",uint8_data);

        uint8_data=nc_slave_apwr_uint8(st,0x12f,uint8_data,&valid);
    printf("uint8_data write:%2x\n",uint8_data);
    // uint16_data=nc_slave_apwr_uint16(st,0x60,uint16_data,&valid);
    // printf("uint16_data write:%2x\n",uint16_data);
    // re_uint16_data=nc_slave_aprd_uint16(st,0x60,&valid);
    // printf("uint16_data read:%2x\n\n",re_uint16_data);
    // uint16_data++;


    // uint32_data=nc_slave_apwr_uint32(st,0x60,uint32_data,&valid);
    // printf("uint32_data write:%2x\n",uint32_data);
    // re_uint32_data=nc_slave_aprd_uint16(st,0x60,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data);
    // uint32_data++;
    usleep(1000);
}


// //寄存器读写函数测试，函数封装了报文生成、发送与接受并简单解析的工作
// uint8 re_data1=nc_slave_aprd_uint8(st,addr,&valid);
// printf("data1:%2x\n",re_data1);

// uint8 re_data2=nc_slave_apwr_uint8(st,0x60,data,&valid);
// printf("data2:%2x\n",re_data2);

// uint8 re_data3=nc_slave_aprd_uint8(st,0x60,&valid);

// printf("data3:%2x\n",re_data3);

//套接字关闭
send_rev_socketClose();
return 0;
}
