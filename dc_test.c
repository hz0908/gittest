#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include "dc_delay.h"

char send_card_name[]="enp1s0";//命令行ifconfig -a 查看
char rev_card_name[]="enp0s31f6";//ifconfig enp0s31f6 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文



// // 测试时间戳报文的功能，能否正常产生时间戳信息
// int main()
// {
// //套接字创建 
// send_socketCreate(send_card_name);
// rev_socketCreate(rev_card_name);

// //发送能产生时间戳的报文
// nc_frame_t frm;
// initframe(&frm);
// timesdgrmAdd(&frm);

// if(FrameTransmit(&frm) < 0)
// return -1;
// if(FrameReceive(&frm) < 0)
// return -1;

// //读寄存器，获取各从站的时间戳信息
// int slave_num = 3;                          // 从站数量，设置不超过3个
// bool flag;
// uint32 slave_timestampRX0[4];
// uint32 slave_timestampRX1[4];
// uint8 slave_timestampTX0[4];
// uint8 slave_timestampTX1[4];
// for(int i=0; i<slave_num; i++)
//     {
//         slave_timestampRX0[i] = nc_slave_aprd_uint32(i,0x30,&flag);
//         slave_timestampRX1[i] = nc_slave_aprd_uint32(i,0x34,&flag);
//         slave_timestampTX0[i] = nc_slave_aprd_uint8(i,0x003C,&flag);
//         slave_timestampTX1[i] = nc_slave_aprd_uint8(i,0x003D,&flag);

//         printf("slave_timestampRX0[%d] = %02x\n",i,slave_timestampRX0[i]);
//         printf("slave_timestampRX1[%d] = %02x\n",i,slave_timestampRX1[i]);
//         printf("slave_timestampTX0[%d] = %02x\n",i,slave_timestampTX0[i]);
//         printf("slave_timestampTX1[%d] = %02x\n",i,slave_timestampTX1[i]);
//         printf("--------------------------------\n");
        
//     }

// //套接字关闭
// send_rev_socketClose();

// return 0;
// }





// 测试延时delay的计算函数接口
int main()
{
//套接字创建 
send_socketCreate(send_card_name);
rev_socketCreate(rev_card_name);

uint8 slave_num = 3;
dcCaldelay(slave_num);

bool flag;
uint32 slave_timestampRX0[4];
uint32 slave_timestampRX1[4];
uint8 slave_timestampTX0[4];
uint8 slave_timestampTX1[4];
for(int i=0; i<slave_num; i++)
    {
        slave_timestampRX0[i] = nc_slave_aprd_uint32(i,0x30,&flag);
        slave_timestampRX1[i] = nc_slave_aprd_uint32(i,0x34,&flag);
        slave_timestampTX0[i] = nc_slave_aprd_uint8(i,0x003C,&flag);
        slave_timestampTX1[i] = nc_slave_aprd_uint8(i,0x003D,&flag);

        printf("slave_timestampRX0[%d] = %02x\n",i,slave_timestampRX0[i]);
        printf("slave_timestampRX1[%d] = %02x\n",i,slave_timestampRX1[i]);
        printf("slave_timestampTX0[%d] = %02x\n",i,slave_timestampTX0[i]);
        printf("slave_timestampTX1[%d] = %02x\n",i,slave_timestampTX1[i]);
        printf("--------------------------------\n");
        
    }

//套接字关闭
send_rev_socketClose();

return 0;
}





// // 测试时钟补偿报文的添加和发送
// int main()
// {
// //套接字创建 
// send_socketCreate(send_card_name);
// rev_socketCreate(rev_card_name);

// nc_frame_t frm;
// uint8 data[8] = {0};
// initframe(&frm);
// dclkdgrmAdd(&frm, DC64COMMAND, data);
// FrameTransmit(&frm);
// FrameReceive(&frm);

// //套接字关闭
// send_rev_socketClose();

// return 0;
// }


