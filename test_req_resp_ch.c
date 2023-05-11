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
#include "reverse_array.h"
#include "channel_view.h"

// 命令行ifconfig -a 查看
// ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文
// gcc bufferSendrecv.c  channel_view.c datagram.c format.c frame.c register.c reverse_array.c service_req_resp_channel.c test_req_resp_ch.c  -o ch
// 测试变量区
char send_card_name[] = "enp1s0";
char rev_card_name[] = "enp2s0";

uint16 ring_position = 0x00;
uint16 phy_addr = 0x0800;
uint16 ch_len = 6;
uint8 control = CH_CONTROL_I;
uint16 ch_No = 0;
uint16 offset = 0x00;
uint16 len = 5;
uint8 data[] = {0x21, 0x11, 0x36, 0x22, 0x33, 0x68};

// 测试程序
int main()
{
    // 套接字创建
    send_socketCreate(send_card_name);
    rev_socketCreate(rev_card_name);

    // data数组倒置
    reverse_array(data, len);

        // 关闭通道0
    service_req_channel_off_apwr(ring_position);

    // 通道0配置
    service_req_channel_config_apwr(ring_position, phy_addr, ch_len, control);

    // 打开通道0
    service_req_channel_on_apwr(ring_position);

     // 查看通道0信息
    channel_view(ring_position, ch_No);

    // 写通道0 5Bytes
    
    service_req_channel_achwr(ring_position, (uint8 *)&data, len, offset);
    // 查看通道0信息
    channel_view(ring_position, ch_No);

  // 写通道0 di6Bytes
  uint16 offset = 0x05;
uint16 len = 1;
    service_req_channel_achwr(ring_position, (uint8 *)&data, len, offset);
    // 查看通道0信息
    channel_view(ring_position, ch_No);


    
    // // 一键配置通道0
    // service_req_channel_config_oneclick(ring_position);

    // // 查看通道0信息
    // channel_view(ring_position, ch_No);

    // // 写通道0 测试
    // service_req_channel_achwr(ring_position, (uint8 *)&data, len, offset);

    // // 查看通道0信息
    // channel_view(ring_position, ch_No);


    // // 查看通道0信息
    // channel_view(ring_position, ch_No);

    // // 通道1配置
    // service_resp_channel_config_apwr(ring_position, phy_addr, ch_len, control);

    // // 打开通道1
    // service_resp_channel_on_apwr(ring_position);



    // // 一键配置通道0
    // service_resp_channel_config_oneclick(ring_position);

    //         // 查看通道0信息
    // channel_view(ring_position, ch_No);

    // // 读通道1 测试
    // service_resp_channel_achrd(ring_position, data, len, offset);
    //       // 查看通道0信息
    // channel_view(ring_position, ch_No);

    // 套接字关闭
    send_rev_socketClose();
    return 0;
}
