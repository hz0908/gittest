#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame.h"
#include "format.h"
#include "string.h"
#include "register.h"
#include "mapping_config.h"
#include "datagram.h"
#include "bufferSendrecv.h"

//映射参数输入
void init_mapping_config(mapping_config_t* config, uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint16 irq_enable, uint16 data_start, uint8 map_count) {
    config->pdo_mapping_ID = MAPNo;
    config->category_ID = CH;
    config->mapping_start_address = MAP_start;
    config->channel_length = LEN;
    config->IRQ_enable = irq_enable;
    config->data_start_address = data_start;
    config->mapping_count = map_count;
}

//顺序号寻址方式通道映射配置函数
bool pdo_mapping_config_by_ring_position(uint16 ring_position,mapping_config_t* mapping0_proc)
{
    bool flag = true;
    // uint8 i = mapping0_proc->mapping_count; //映射号
    uint16 temp;
    // for(i=0;i<mapping0_proc->mapping_count;i++)
    // {
        //Mapping类型初始化配置
        nc_slave_apwr_uint16(ring_position, 0x0284 + (mapping0_proc->mapping_count)*8, 0x0000, &flag);
        if(!flag)
            return false;
        
        //Mapping编号配置
        nc_slave_apwr_uint16(ring_position, 0x0280 + (mapping0_proc->mapping_count)*8, mapping0_proc->pdo_mapping_ID, &flag);
        if(!flag)
            return false;

        //Mapping标识寄存器配置 
        temp = mapping0_proc->category_ID&0xFF;    //temp为通道号
        temp = (temp << 11) | (mapping0_proc->mapping_start_address & 0x7FF);
        nc_slave_apwr_uint16(ring_position, 0x0282 + (mapping0_proc->mapping_count)*8, temp, &flag);
        if(!flag)
           return false;

        //Mapping类型配置
        temp = 0x8000;//1000  0000  0000  0000
        if((mapping0_proc->IRQ_enable & 0x10) == 0x10)
            temp |= (0x00 << 10);
        else
            temp |= (0x10 << 10);
         //temp |= ((slave->pdo_channel_list[i].IRQ_enable & 0x10) >> 10);
        temp |= (mapping0_proc->channel_length & 0x7FF);
        nc_slave_apwr_uint16(ring_position, 0x0284 + (mapping0_proc->mapping_count)*8, temp, &flag);
        if(!flag)
            return false;
        
        //Mapping通道内地址配置
        nc_slave_apwr_uint16(ring_position, 0x0286 + (mapping0_proc->mapping_count)*8, mapping0_proc->data_start_address, &flag);
        if(!flag)
           return false;
 }



