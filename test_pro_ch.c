#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include "process_data_channel.h"
#include "service_data_channel.h"
#include "service_req_resp_channel.h"
#include "channel_config_parameter.h"
#include "channel_view.h"

// 命令行ifconfig -a 查看
// ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文
// gcc bufferSendrecv.c test_pro_ch.c datagram.c channel_view.c format.c frame.c process_data_channel.c register.c reverse_array.c  -o ch
// 测试变量区
char send_card_name[] = "enp1s0";
char rev_card_name[] = "enp2s0";

uint16 ring_position = 0x00;
uint16 phy_addr = 0x0800;
uint16 ch_len = 6;
uint8 control = CH_CONTROL_SI;
uint8 ch_No = 2;
uint16 offset = 0x00;
uint16 len = 6;
uint8 data[] = {0x11, 0x86, 0x36, 0x22, 0x33, 0x68};

// 测试程序
int main()
{
  // 套接字创建
  send_socketCreate(send_card_name);
  rev_socketCreate(rev_card_name);

  // data数组倒置
  //reverse_array(data, len);

  // // 通道配置
  // proc_data_channel_config_apwr(ring_position, phy_addr, ch_len, control, ch_No);

  // // 打开通道
  // proc_data_channel_on_apwr(ring_position, ch_No);

  // 查看通道信息
  channel_view(ring_position, ch_No);

  // // 写通道 测试
  // proc_data_channel_achwr(ring_position, (uint8 *)&data, len, ch_No, offset); // 六字节通道写0-4五个字节 成功
  // print_data(data, len);

  // // 查看通道信息
  // channel_view(ring_position, ch_No);

  // // 关闭通道
  // proc_data_channel_off_apwr(ring_position, ch_No);

  // // 同时配置第二个通道完成读写
  // phy_addr = 0x0800;
  // ch_No = 3;
  // proc_data_channel_config_apwr(ring_position, phy_addr, ch_len, control, ch_No);
  // proc_data_channel_on_apwr(ring_position, ch_No);
  // proc_data_channel_achwr(ring_position, (uint8 *)&data, len, ch_No, offset);

  // proc_data_channel_achwr(ring_position, (uint8 *)&data, len, ch_No, offset);//六字节通道再次写0-4五个字节 失败
  // offset = 0x05;
  // len = 1;
  // char data1[] = {0x11};
  // proc_data_channel_achwr(ring_position, (uint8 *)&data1, len, ch_No, offset);//六字节通道写5一个字节 成功
  // offset = 0x00;
  // len = 5;
  // char data2[]= {0x11, 0x36, 0x22, 0x33, 0x68};
  // reverse_array(data2, len);
  // proc_data_channel_achwr(ring_position, (uint8 *)&data2, len, ch_No, offset);//六字节通道写0-4五个字节 成功

  // 套接字关闭
  send_rev_socketClose();
  return 0;
}
