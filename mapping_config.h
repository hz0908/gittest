#include "format.h"
#include <string.h>

typedef struct mapping_config mapping_config_t;
struct mapping_config{
    uint16 pdo_mapping_ID;//映射编号-MAPNo
    uint16 category_ID;//通道号-CH
    uint16 mapping_start_address;//报文内起始地址
    uint16 channel_length;//数据长度
    uint16  IRQ_enable;//通道使能位+读写方向位
    uint16 data_start_address;//数据在通道内的起始地址
    uint8 mapping_count;//映射通道编号
	//uint16 channel_start_address;
	//uint8  channel_enable;
    //pdo_object_mapping_t * pdo_mapping;
};

void init_mapping_config(mapping_config_t* config, uint16 MAPNo, uint16 CH, uint16 MAP_start, uint16 LEN, uint16 irq_enable, uint16 data_start, uint8 map_count);
bool pdo_mapping_config_by_ring_position(uint16 ring_position,mapping_config_t* mapping);
