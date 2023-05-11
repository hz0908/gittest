// 过程数据通道
#ifndef PROCESS_DATA_CHANNEL
#define PROCESS_DATA_CHANNEL

#include "type.h"
#include "register.h"
#include "frame.h"

// 过程数据通道:通道号最小值和最大值
#define MINI_PRO_CHANNEL_NO 2
#define MAX_PRO_CHANNEL_NO 11

// 过程数据通道管理器配置区大小间隔，16字节
#define PRO_CHANNEL_CONFIG_SIZE 16

// 1.通道配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool proc_data_channel_config(uint16 station_address, uint16 phy_addr,
                              uint16 len, uint8 control, uint8 ch_No);
// 顺序寻址
bool proc_data_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                   uint16 len, uint8 control, uint8 ch_No);

// 2.过程数据通道开关
// 站地址寻址
bool proc_data_channel_on(uint16 station_address, uint8 ch_No);
bool proc_data_channel_off(uint16 station_address, uint8 ch_No);
// 顺序选址
bool proc_data_channel_off_apwr(uint16 ring_position, uint8 ch_No);
bool proc_data_channel_on_apwr(uint16 ring_position, uint8 ch_No);

// 3.通道读写函数
// 站地址寻址
bool proc_data_channel_fchrd(uint16 station_address, uint8 *data, uint16 len, uint8 ch_No, uint16 offset);
bool proc_data_channel_fchwr(uint16 station_address, uint8 *data, uint16 len, uint8 ch_No, uint16 offset);
// 顺序寻址
bool proc_data_channel_achrd(uint16 ring_position, uint8 *data, uint16 len, uint8 ch_No, uint16 offset);
bool proc_data_channel_achwr(uint16 ring_position, uint8 *data, uint16 len, uint8 ch_No, uint16 offset);

#endif
