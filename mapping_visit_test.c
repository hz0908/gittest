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

    /**开通道**/
    /**开启过程数据通道CH=2**/




    /**开启过程数据通道CH=2**/



    /**配置映射**/
    /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/
    mapping_config_t *mapping0_proc = malloc(sizeof(mapping_config_t)); // 动态分配内存
    memset(mapping0_proc, 0, sizeof(mapping_config_t));

    // uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint8 irq_enable, uint16 data_start, uint8 map_count
    init_mapping_config(mapping0_proc, 0x01, 0x02, 0x00, 0x04, 0x00, 0x00, 0x02); // IRQ0x00为写，0x10为读
    pdo_mapping_config_by_ring_position(st, mapping0_proc);

    uint32 re_uint32_data1 = nc_slave_aprd_uint32(st, 0x2a0, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data1);

    uint32 re_uint32_data2 = nc_slave_aprd_uint32(st, 0x284 + (mapping0_proc->mapping_count) * 8, &valid);
    printf("uint32_data read:%2x\n\n", re_uint32_data2);

    free(mapping0_proc);
    /**配置过程数据通道CH=2映射，映射方向写，可测试通道主站写从站读方向**/



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
    /**通过通道映射方式向通道RAM区进行写操作**/

    // 套接字关闭
    send_rev_socketClose();

    return 0;
}