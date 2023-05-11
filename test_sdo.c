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
#include "slave.h"
#include "type.h"
#include "sdo.h"

// 命令行ifconfig -a 查看
// ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文
// gcc bufferSendrecv.c  channel_view.c datagram.c format.c frame.c register.c reverse_array.c service_req_resp_channel.c slave.c sdo.c sdo_prepare.c test_sdo.c -o download
// 测试变量区
char send_card_name[] = "enp1s0";
char rev_card_name[] = "enp2s0";

uint16 ring_position = 0x00;
uint16 phy_addr0 = 0x0800;
uint16 ch_len0 = 0x20;
uint16 phy_addr1 = 0x0820;
uint16 ch_len1 = 0x20;
uint8 control = CH_CONTROL_I;

// 测试程序
int main()
{
    // 套接字创建
    send_socketCreate(send_card_name);
    rev_socketCreate(rev_card_name);

    // 关闭通道0
    service_req_channel_off_apwr(ring_position);
    // 关闭通道1
    service_resp_channel_off_apwr(ring_position);

    // 通道0配置
    service_req_channel_config_apwr(ring_position, phy_addr0, ch_len0, control);
    // 打开通道0
    service_req_channel_on_apwr(ring_position);

    // // 查看通道0信息
    // channel_view(ring_position, 0);

    // 通道1配置
    service_resp_channel_config_apwr(ring_position, phy_addr1, ch_len1, control);
    // 打开通道1
    service_resp_channel_on_apwr(ring_position);

    // // 查看通道1信息
    // channel_view(ring_position, 1);

    // // SDO Download test
    // nc_slave_t *slave = (nc_slave_t *)malloc(sizeof(nc_slave_t));
    // slave_basic_info_t *basic_info = (slave_basic_info_t *)malloc(sizeof(slave_basic_info_t));
    // basic_info->Service_req_channel_length = 0x11;
    // basic_info->Service_res_channel_length = 0x11;
    // basic_info->Channel_Protocol = 01;
    // slave->basic_info = basic_info;
    // slave->ring_position = 0;
    // uint8 data[] = {0x88, 0x11, 0x22, 0x59,0x89, 0x11, 0x22, 0x59};
    // uint16 index = 0x1601;
    // uint8 subindex = 1;
    // uint8 number = 1;
    // uint8 length = 2;
    // SDO_Download(slave, data, index, subindex, number, length);

    // // SDO Upload test
    // nc_slave_t *slave = (nc_slave_t *)malloc(sizeof(nc_slave_t));
    // slave_basic_info_t *basic_info = (slave_basic_info_t *)malloc(sizeof(slave_basic_info_t));
    // basic_info->Service_req_channel_length = 0x10;
    // basic_info->Service_res_channel_length = 0x10;
    // basic_info->Channel_Protocol = 01;
    // slave->basic_info = basic_info;
    // slave->ring_position = 0;
    // uint8 length = 8;
    // uint8 *data = (uint8 *)malloc(length);
    // uint16 index = 0x1601;
    // uint8 subindex = 1;
    // uint8 number = 2;
    // SDO_Upload(slave, data, index, subindex, number);
    // // //打印data
    // // printf("data:0x");
    // // for (int i = 0; i < length; i++)
    // //     printf("%x", data[i]);
    // // printf("\n");

    // SDO information test
    nc_slave_t *slave = (nc_slave_t *)malloc(sizeof(nc_slave_t));
    ListHead_t *dictsionary = (ListHead_t *)malloc(sizeof(ListHead_t));
    slave_basic_info_t *basic_info = (slave_basic_info_t *)malloc(sizeof(slave_basic_info_t));
    basic_info->Service_req_channel_length = ch_len0;
    basic_info->Service_res_channel_length = ch_len1;
    basic_info->Channel_Protocol = 01;
    slave->basic_info = basic_info;
    slave->ring_position = 0;
    slave->dictsionary = dictsionary;
    Sdo_Info_Get(slave);

    // 套接字关闭
    send_rev_socketClose();
    return 0;
}
