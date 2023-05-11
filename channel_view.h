// 过程数据通道
#ifndef CHANNEL_VIEW
#define CHANNEL_VIEW

#include "type.h"

struct ch_config_inf
{
    uint16 ch_start_addr;
    uint16 ch_len;
    uint8 ch_interrrupt_switch;
    uint8 ch_switch;
    uint8 ch_status;
    uint16 ch_M_offset;
    uint16 ch_S_addr;
    uint16 ch_M_Bytes;
    uint16 ch_S_Bytes;
};

struct ch_config_inf channel_view(uint16 ring_position, uint8 ch_No);

#endif 