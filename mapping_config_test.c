#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include "mapping_config.h"

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

    /**配置过程数据通道CH=2映射，映射方向写，配置首地址为0x280**/
    mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00); // IRQ0x00为写，0x10为读
    pdo_mapping_config_by_ring_position(st, mapping0_proc);

    uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x280 + (mapping0_proc->mapping_count) * 8, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data1);

    uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data2);

    free(mapping0_proc);
    // /**配置过程数据通道CH=2映射，映射方向写，配置首地址为0x280**/

    // /**配置过程数据通道CH=2映射，映射方向读，配置首地址为0x280**/
    // mapping_config_t* mapping1_proc = malloc(sizeof(mapping_config_t));//动态分配内存
    // memset(mapping1_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping1_proc, 0x01, 0x02, 0x00, 0x04, 0x10, 0x00, 0x00);//IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping1_proc);

    // uint32 re_uint32_data3=nc_slave_aprd_uint32(st,0x280 + (mapping1_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data3);

    // uint32 re_uint32_data4=nc_slave_aprd_uint32(st,0x284 + (mapping1_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data4);

    // free(mapping1_proc);
    // /**配置过程数据通道CH=2映射，映射方向读，配置首地址为0x280**/

    //  /**配置过程数据通道CH=3映射，映射方向读，配置首地址为0x280**/
    // mapping_config_t* mapping2_proc = malloc(sizeof(mapping_config_t));//动态分配内存
    // memset(mapping2_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping2_proc, 0x01, 0x03, 0x00, 0x04, 0x10, 0x00, 0x00);//IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping2_proc);

    // uint32 re_uint32_data5=nc_slave_aprd_uint32(st,0x280 + (mapping2_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data5);

    // uint32 re_uint32_data6=nc_slave_aprd_uint32(st,0x284 + (mapping2_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data6);

    // free(mapping2_proc);
    // /**配置过程数据通道CH=3映射，映射方向读，配置首地址为0x280**/

    //  /**配置过程数据通道CH=0映射，映射方向读，配置首地址为0x280**/
    // mapping_config_t* mapping0_proc = malloc(sizeof(mapping_config_t));//动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00);//IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data7=nc_slave_aprd_uint32(st,0x280 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data7);

    // uint32 re_uint32_data8=nc_slave_aprd_uint32(st,0x284 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data8);

    // free(mapping0_proc);
    // /**配置过程数据通道CH=0映射，映射方向读，配置首地址为0x280**/

    //  /**配置服务请求通道CH=2映射，映射方向读，配置首地址为0x288**/
    // mapping_config_t* mapping0_proc = malloc(sizeof(mapping_config_t));//动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x10, 0x00, 0x01);//IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data9=nc_slave_aprd_uint32(st,0x280 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data9);

    // uint32 re_uint32_data10=nc_slave_aprd_uint32(st,0x284 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data10);

    // free(mapping0_proc);
    // /**配置服务请求通道CH=2映射，映射方向读，配置首地址为0x288**/

    //  /**配置服务请求通道CH=2映射，映射方向读，配置首地址为0x290**/
    // mapping_config_t* mapping0_proc = malloc(sizeof(mapping_config_t));//动态分配内存
    // memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // // uint16 map_no, uint16 channel_no, uint16 map_start, uint16 channel_len, uint8 irq_enable, uint16 data_start
    // init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x10, 0x00, 0x02);//IRQ0x00为写，0x10为读
    // pdo_mapping_config_by_ring_position(st, mapping0_proc);

    // uint32 re_uint32_data11=nc_slave_aprd_uint32(st,0x280 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data11);

    // uint32 re_uint32_data12=nc_slave_aprd_uint32(st,0x284 + (mapping0_proc->mapping_count)*8,&valid);
    // printf("uint32_data read:%2x\n\n",re_uint32_data12);

    // free(mapping0_proc);
    // /**配置服务请求通道CH=2映射，映射方向读，配置首地址为0x290**/

    // 套接字关闭
    send_rev_socketClose();

    return 0;
}