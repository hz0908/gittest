#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"


//测试变量区
char send_card_name[]="enp4s0";//命令行ifconfig -a 查看
char rev_card_name[]="enp3s0";//ifconfig enp3s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文

uint16 st=0;
uint16 addr=0x0290;
// uint16 datalen=0x04;
// uint8 data=0x12;
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


//寄存器读写函数测试，函数封装了报文生成、发送与接受并简单解析的工作
uint8 re_data1=nc_slave_aprd_uint8(st,addr,&valid);
printf("data1:%2x\n",re_data1);
//读固定值寄存器的值

// uint8 wr_data2=nc_slave_apwr_uint8(st,0x60,data,&valid);
// printf("data2:%2x\n",wr_data2);

// uint8 re_data3=nc_slave_aprd_uint8(st,0x60,&valid);

// printf("data3:%2x\n",re_data3);

//套接字关闭
send_rev_socketClose();
return 0;
}