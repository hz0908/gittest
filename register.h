#ifndef REGISTER
#define REGISTER
#include "type.h"

#define REG_VENDOR_ID       0x0000
#define REG_PRODUCT_CODE    0x0002
#define REG_VERSION         0x0004
#define REG_REVISION        0x0006
#define REG_PORT_TYPE       0x0008
#define REG_DCLK_NUM        0x000A
#define REG_PROC_CHANNEL_NUM 0x000B
#define REG_SVR_CHANNEL_NUM 0x000C
#define REG_FIFO_NUM        0x000D
#define REG_MAP_UNIT_NUM    0x000E
#define REG_RAM_SIZE        0x000F


#define REG_PORT_STATE      0x0022


#define REG_STATION_ADDR    0x0010
#define REG_SLAVE_RESET     0x0012

/**SYNC相关寄存器地址**/
#define REG_SYNC_MODE   0x0060
#define REG_SYNC_DIVIDER_FACTOR 0x0061
#define REG_SYNC_PHASE_FACTOR   0x0062
#define REG_SYNC_CYCLE_DELAY    0x0063

#define REG_SYNC_PULSE_WIDTH    0x0066
#define REG_SYNC_STARTTIME_LO   0x0068
#define REG_SYNC_STARTTIME_HI   (REG_SYNC_STARTTIME_LO + 4)


/***服务请求/应答通道配置区基址***/
#define SVR_REQ_CH_ADDR_REG 0x0100
#define SVR_REQ_CH_LEN_REG  0x0102
#define SVR_REQ_CH_INT_REG  0x0104
#define SVR_REQ_CH_VALID_REG    0x0105
#define SVR_REQ_CH_STATUS_REG   0x0107

#define SVR_RESP_CH_ADDR_REG 0x0110
#define SVR_RESP_CH_LEN_REG  0x0112
#define SVR_RESP_CH_INT_REG  0x0114
#define SVR_RESP_CH_VALID_REG    0x0115
#define SVR_RESP_CH_STATUS_REG   0x0117


/**过程数据通道配置区基地址**/
#define REG_BASE_PRO_CHANNEL_PHY_ADDRESS    0x0120
#define REG_BASE_PRO_CHANNEL_LEN    0x0122
#define REG_BASE_PRO_CHANNEL_CONTROL    0x0124
#define REG_BASE_PRO_CHANNEL_SWITCH     0x0125

//服务数据通道配置区基地址
#define SVR_DATA_CH_ADDR_REG    0x01C0
#define SVR_DATA_CH_LEN_REG    0x01C2
#define SVR_DATA_CH_CTRL_REG    0x01C4
#define SVR_DATA_CH_SWITCH_REG    0x01C5

#define REG_STATE_REQ       0x0300
#define REG_STATE_CURRENT   0x0301

/**FIFO通道，可读写字节数**/
#define REG_FIFO_RW_BYTE_NUM    0x0328





/***通道号定义***/
/**服务请求通道 和 服务应答通道 通道号 **/
#define SVR_REQ_CH  0x00
#define SVR_RESP_CH 0x01

/****顺序方式访问从站****/
/*****valid为真时，访问成功****/
uint8 nc_slave_aprd_uint8(uint16 ring_position,uint16 offset,bool* valid);
uint8 nc_slave_apwr_uint8(uint16 ring_position,uint16 offset,uint8 byte,bool* valid);
uint16 nc_slave_aprd_uint16(uint16 ring_position,uint16 offset,bool* valid);
uint16 nc_slave_apwr_uint16(uint16 ring_position,uint16 offset,uint16 word,bool* valid);
uint32 nc_slave_aprd_uint32(uint16 ring_position,uint16 offset,bool* valid);
uint32 nc_slave_apwr_uint32(uint16 ring_position,uint16 offset,uint32 Dword,bool* valid);

/****顺序寻址方式转为通道配置的6byte寄存器读写****/
uint8* nc_slave_aprd_6byte(uint16 ring_position,uint16 offset,uint8* data,bool* valid);
uint8* nc_slave_apwr_6byte(uint16 ring_position,uint16 offset,uint8* data,bool* valid);


/****配置站地址方式访问从站****/
uint8 nc_slave_fprd_uint8(uint16 station_address,uint16 offset,bool* valid);
uint8 nc_slave_fpwr_uint8(uint16 station_address,uint16 offset,uint8 byte,bool* valid);
uint16 nc_slave_fprd_uint16(uint16 station_address,uint16 offset,bool* valid);
uint16 nc_slave_fpwr_uint16(uint16 station_address,uint16 offset,uint16 word,bool* valid);
uint32 nc_slave_fprd_uint32(uint16 station_address,uint16 offset,bool* valid);
uint32 nc_slave_fpwr_uint32(uint16 station_address,uint16 offset,uint32 Dword,bool* valid);

/***广播方式访问从站结点***/
uint8 nc_slave_brd_uint8(uint16 offset,bool* valid);
uint8 nc_slave_bwr_uint8(uint16 offset,uint8 byte,bool* valid);
uint16 nc_slave_brd_uint16(uint16 offset,bool* valid);
uint16 nc_slave_bwr_uint16(uint16 offset,uint16 word,bool* valid);
uint32 nc_slave_brd_uint32(uint16 offset,bool* valid);
uint32 nc_slave_bwr_uint32(uint16 offset,uint32 Dword,bool* valid);

// 任意字节 寄存器 读 顺序寻址
uint8 *nc_slave_aprd_all(uint16 ring_position, uint16 offset, uint8 *data, bool *valid,uint16 len);

#endif // REGISTER

