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
#include "mapping_visit.h"
#include "process_data_channel.h"
#include "service_data_channel.h"
#include "service_req_resp_channel.h"
#include "channel_config_parameter.h"
#include "reverse_array.h"
#include "channel_view.h"


// 测试变量区
char send_card_name[] = "enp4s0"; // 命令行ifconfig -a 查看
char rev_card_name[] = "enp3s0";  // ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文

uint16 st = 0;
// uint16 addr=0x00;
// uint16 datalen=0x04;
// uint8 data=0x12;
// uint16 irq=0;
bool valid = true;

// 测试程序
int main()
{
    // 套接字创建
    send_socketCreate(send_card_name);
    rev_socketCreate(rev_card_name);

    // 帧初始化
    nc_frame_t frm;
    initframe(&frm);

    // /**测试一**/
    // /**开通道**/
    // /**开启过程数据通道CH=2**/
    // uint16 phy_addr = 0x0800;
    // uint16 ch_len = 4;
    // uint8 control = CH_CONTROL_MI;//主站写从站读
    // uint8 ch_No = 2;
    // // uint16 offset = 0x00;
    // // uint16 len = 5;
    // // char data[] = {0x11, 0x36, 0x22, 0x33, 0x68};

    // // data数组倒置
    // // reverse_array(data, len);
    // // 通道配置
    // proc_data_channel_config_apwr(st, phy_addr, ch_len, control, ch_No);
    // // 打开通道
    // proc_data_channel_on_apwr(st, ch_No);
    // //查看通道状态寄存器
    // uint8 re_uint8_data1 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data1);
    // /**开启过程数据通道CH=2**/


    // /**配置映射**/
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/
    // mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00); // IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data1);

    // uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data2);

    // free(mapping0_proc);
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/


    // /**映射访问**/
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // uint16 MAPNo = 0x01;
    // uint8 data[] = {0x11, 0x22, 0x33, 0x66};
    // uint16 len = sizeof(data) / sizeof(data[0]);

    // // 调用mapping_singlecycle_wr函数进行写操作
    // if (mapping_singlecycle_wr(MAPNo, len, data)) 
    // {
    //     printf("Data written successfully: ");
    //     printData(data, len);
    // } else 
    // {
    //     printf("Failed to write data.\n");
    // }
    // //查看通道状态寄存器
    // uint8 re_uint8_data2 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data2);
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // /**测试一**/



    // /**测试二**/
    // /**开通道**/
    // /**开启过程数据通道CH=2**/
    // uint16 phy_addr = 0x0800;
    // uint16 ch_len = 4;
    // uint8 control = CH_CONTROL_MI;//主站写从站读
    // uint8 ch_No = 2;
    // uint16 offset = 0x00;
    // uint16 Len = 4;
    // char data0[] = {0x11, 0x22, 0x33, 0x66};
    // // data数组倒置
    // // reverse_array(data, len);
    // // 通道配置
    // proc_data_channel_config_apwr(st, phy_addr, ch_len, control, ch_No);
    // // 打开通道
    // proc_data_channel_on_apwr(st, ch_No);
    // //查看通道状态寄存器
    // uint8 re_uint8_data1 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data1);
    // //写通道
    // proc_data_channel_achwr(st, (uint8 *)&data0, Len, ch_No, offset);
    // // print_data(data, len);
    // //查看通道状态寄存器
    // uint8 re_uint8_data2 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data2);
    // /**开启过程数据通道CH=2**/


    // /**配置映射**/
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/
    // mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x10, 0x00, 0x00); // IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data1);

    // uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data2);

    // free(mapping0_proc);
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/


    // /**映射访问**/
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // uint16 MAPNo = 0x01;
    // uint8 data[4];
    // uint16 len = 4;
    // // 调用 mapping_singlecycle_rd 函数进行读操作
    // if (mapping_singlecycle_rd(MAPNo, len, data))
    // {
    //     printf("Mapping read successfully.\n");
    // }
    // else
    // {
    //     printf("Mapping read failed.\n");
    // }
    // //查看通道状态寄存器
    // uint8 re_uint8_data3 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data3);
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // /**测试二**/



    /**测试三**/
    /**开通道**/
    /**开启服务请求通道CH=0**/
    uint16 phy_addr = 0x0800;
    uint16 ch_len = 4;
    uint8 control = CH_CONTROL_I;
    uint8 ch_No = 0;
    // uint16 offset = 0x00;
    // uint16 len = 5;
    // char data[] = {0x11, 0x36, 0x22, 0x33, 0x68};
    // data数组倒置
    // reverse_array(data, len);
    // 关闭通道0
    service_req_channel_off_apwr(st);
    // 通道0配置
    service_req_channel_config_apwr(st, phy_addr, ch_len, control);
    // 打开通道0
    service_req_channel_on_apwr(st);
    //查看通道状态寄存器
    uint8 re_uint8_data1 = nc_slave_aprd_uint8(st, 0x107, &valid);
    printf("uint8_data read:%2x\n\n", re_uint8_data1);
    /**开启过程数据通道CH=2**/


    /**配置映射**/
    /**配置服务请求通道CH=0映射，映射方向写**/
    mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    init_mapping_config(mapping0_proc, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00); // IRQ0x00为写，0x10为读
    pdo_mapping_config_by_ring_position(st, mapping0_proc);

    uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data1);

    uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data2);

    free(mapping0_proc);
    /**配置过程数据通道CH=0映射，映射方向写**/


    /**映射访问**/
    /**通过通道映射方式向通道RAM区进行写操作**/
    uint16 MAPNo = 0x01;
    uint8 data[] = {0x11, 0x22, 0x33, 0x66};
    uint16 len = sizeof(data) / sizeof(data[0]);

    // 调用mapping_singlecycle_wr函数进行写操作
    if (mapping_singlecycle_wr(MAPNo, len, data)) 
    {
        printf("Data written successfully: ");
        printData(data, len);
    } else 
    {
        printf("Failed to write data.\n");
    }
    //查看通道状态寄存器
    uint8 re_uint8_data2 = nc_slave_aprd_uint8(st, 0x107, &valid);
    printf("uint8_data read:%2x\n\n", re_uint8_data2);
    /**通过通道映射方式向通道RAM区进行写操作**/
    /**测试三**/



    // /**测试四**/
    // /**开通道**/
    // /**开启过程数据通道CH=2**/
    // uint16 phy_addr = 0x0800;
    // uint16 ch_len = 4;
    // uint8 control = CH_CONTROL_MI;//主站写从站读
    // uint8 ch_No = 2;
    // // uint16 offset = 0x00;
    // // uint16 len = 5;
    // // char data[] = {0x11, 0x36, 0x22, 0x33, 0x68};

    // // data数组倒置
    // // reverse_array(data, len);
    // // 通道配置
    // proc_data_channel_config_apwr(st, phy_addr, ch_len, control, ch_No);
    // // 打开通道
    // proc_data_channel_on_apwr(st, ch_No);
    // //查看通道状态寄存器
    // uint8 re_uint8_data1 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data1);
    // /**开启过程数据通道CH=2**/


    // /**配置映射**/
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/
    // mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00); // IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data1);

    // uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data2);

    // free(mapping0_proc);
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/


    // /**映射访问**/
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // uint16 MAPNo = 0x02;
    // uint8 data[] = {0x11, 0x22, 0x33, 0x66};
    // uint16 len = sizeof(data) / sizeof(data[0]);

    // // 调用mapping_singlecycle_wr函数进行写操作
    // if (mapping_singlecycle_wr(MAPNo, len, data)) 
    // {
    //     printf("Data written successfully: ");
    //     printData(data, len);
    // } else 
    // {
    //     printf("Failed to write data.\n");
    // }
    // //查看通道状态寄存器
    // uint8 re_uint8_data2 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data2);
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // /**测试四**/

    // /**测试五**/
    // /**开通道**/
    // /**开启过程数据通道CH=2**/
    // uint16 phy_addr = 0x0800;
    // uint16 ch_len = 4;
    // uint8 control = CH_CONTROL_MI;//主站写从站读
    // uint8 ch_No = 2;
    // // uint16 offset = 0x00;
    // // uint16 len = 5;
    // // char data[] = {0x11, 0x36, 0x22, 0x33, 0x68};

    // // data数组倒置
    // // reverse_array(data, len);
    // // 通道配置
    // proc_data_channel_config_apwr(st, phy_addr, ch_len, control, ch_No);
    // // 打开通道
    // proc_data_channel_on_apwr(st, ch_No);
    // /**开启过程数据通道CH=2**/


    // /**配置映射**/
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/
    // mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x00, 0x04, 0x00); // IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data1);

    // uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    // printf("uint32_data read:%2x\n\n", re_uint32_data2);

    // free(mapping0_proc);
    // /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/


    // /**映射访问**/
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // uint16 MAPNo = 0x01;
    // uint8 data[] = {0x11, 0x22, 0x33, 0x66};
    // uint16 len = sizeof(data) / sizeof(data[0]);

    // // 调用mapping_singlecycle_wr函数进行写操作
    // if (mapping_singlecycle_wr(MAPNo, len, data)) 
    // {
    //     printf("Data written successfully: ");
    //     printData(data, len);
    // } else 
    // {
    //     printf("Failed to write data.\n");
    // }
    // //查看通道状态寄存器
    // uint8 re_uint8_data2 = nc_slave_aprd_uint8(st, 0x127, &valid);
    // printf("uint8_data read:%2x\n\n", re_uint8_data2);
    // /**通过通道映射方式向通道RAM区进行写操作**/
    // /**测试五**/


    // 套接字关闭
    send_rev_socketClose();

    return 0;
}