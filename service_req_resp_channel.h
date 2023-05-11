// 服务请求/应答通道
#ifndef SERVICE_REQ_RESP_CHANNEL
#define SERVICE_REQ_RESP_CHANNEL

#include "type.h"
#include "register.h"
#include "frame.h"

// 通道状态
typedef enum channel_status
{
    FULL,
    EMPTY,
    UNDEFINE
} channel_status_t;

// 服务请求通道

// 0.1.通道0 一键配置
bool service_req_channel_config_oneclick(uint16 ring_position);

// 0.3. 查看配置信息
struct ch_config_inf channel_view(uint16 ring_position, uint8 ch_No);

// 1.通道0 服务请求通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool service_req_channel_config_fpwr(uint16 station_address, uint16 phy_addr,
                                     uint16 len, uint8 control);
// 顺序寻址
bool service_req_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                     uint16 len, uint8 control);

// 2.服务请求通道开关
// 站地址寻址
bool service_req_channel_on_fpwr(uint16 station_address);
bool service_req_channel_off_fpwr(uint16 station_address);
// 顺序寻址
bool service_req_channel_on_apwr(uint16 ring_position);
bool service_req_channel_off_apwr(uint16 ring_position);

// 3.服务请求通道写函数
// 站地址寻址
bool service_req_channel_fchwr(uint16 station_address, uint8 *data, uint16 len, uint16 offset);
// 顺序寻址
bool service_req_channel_achwr(uint16 ring_position, uint8 *data, uint16 len, uint16 offset);

// 服务应答通道

// 0.2.通道1 一键配置
bool service_resp_channel_config_oneclick(uint16 ring_position);

// 4.通道1 服务应答通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool service_resp_channel_config_fpwr(uint16 station_address, uint16 phy_addr,
                                      uint16 len, uint8 control);
// 顺序寻址
bool service_resp_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                      uint16 len, uint8 control);

// 5.服务应答通道开关
// 站地址寻址
bool service_resp_channel_on_fpwr(uint16 station_address);
bool service_resp_channel_off_fpwr(uint16 station_address);
// 顺序寻址
bool service_resp_channel_on_apwr(uint16 ring_position);
bool service_resp_channel_off_apwr(uint16 ring_position);

// 6.服务应答通道读函数
// 站地址寻址
bool service_resp_channel_fchrd(uint16 station_address, uint8 *data, uint16 len, uint16 offset);
// 顺序寻址
bool service_resp_channel_achrd(uint16 ring_position, uint8 *data, uint16 len, uint16 offset);

// 7.检测服务应答通道是否有数据，是否为满，是否可读
channel_status_t status_service_channel_respond_by_ring_position(uint16 ring_position);

// 8.读取服务应答通道的数据，将数据输出，data接收读出字节的首地址，由调用者保证data的区域有足够的空间接收读出的数据
bool service_channel_respond_by_ring_position(uint16 ring_position, uint16 len_ch, uint8 *data);

// 9.获取服务应答通道的数据
uint8 *nc_slave_res_channel_fetch(const nc_slave_t *slave, uint8 *datagram, uint8 *type, uint16 *size);
#endif
