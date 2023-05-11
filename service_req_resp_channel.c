#include "service_req_resp_channel.h"
#include "datagram.h"
#include <string.h>
#include "format.h"
#include "reverse_array.h"

// 服务请求通道

// 0.1.通道0 一键配置
bool service_req_channel_config_oneclick(uint16 ring_position)
{
    uint8 data[6];
    memset(data, 0, 6);
    reverse_array(data, 6);
    bool valid = true;
    nc_slave_aprd_all(ring_position, 0x424, (uint8 *)&data, &valid, 6);
    print_data(data, 6);
    uint16 phy_addr = (uint16)data[0] | (uint16)data[1] << 8;
    uint16 ch_len = (uint16)data[2] | (uint16)data[3] << 8;
    uint8 control = (uint16)data[4];
    service_req_channel_config_apwr(ring_position, phy_addr, ch_len, control);
    if (data[5] == 0x80)
    {
        service_req_channel_on_apwr(ring_position);
    }
    else if (data[5] == 0x00)
    {
        service_req_channel_off_apwr(ring_position);
    }
    else
    {
        printf("通道0启停配置区有错误\n");
    }
    return true;
}

// 1.通道0 服务请求通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool service_req_channel_config_fpwr(uint16 station_address, uint16 phy_addr,
                                     uint16 len, uint8 control)
{
    bool flag; // 检验成功与否的标志位

    nc_slave_fpwr_uint16(station_address, SVR_REQ_CH_ADDR_REG, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint16(station_address, SVR_REQ_CH_LEN_REG, len, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint8(station_address, SVR_REQ_CH_INT_REG, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 1.通道0 服务请求通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 顺序寻址
bool service_req_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                     uint16 len, uint8 control)
{
    bool flag;

    nc_slave_apwr_uint16(ring_position, SVR_REQ_CH_ADDR_REG, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint16(ring_position, SVR_REQ_CH_LEN_REG, len, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint8(ring_position, SVR_REQ_CH_INT_REG, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 2.服务请求通道开关
// 站地址寻址
bool service_req_channel_on_fpwr(uint16 station_address)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0105
    nc_slave_fpwr_uint8(station_address, SVR_REQ_CH_VALID_REG, 0x80, &flag);

    if (!flag)
        return false;

    return true;
}

bool service_req_channel_off_fpwr(uint16 station_address)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0105
    nc_slave_fpwr_uint8(station_address, SVR_REQ_CH_VALID_REG, 0x00, &flag);

    if (!flag)
        return false;

    return true;
}

// 2.服务请求通道开关
// 顺序寻址
bool service_req_channel_on_apwr(uint16 ring_position)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0105
    nc_slave_apwr_uint8(ring_position, SVR_REQ_CH_VALID_REG, 0x80, &flag);
    if (!flag)
        return false;

    return true;
}

bool service_req_channel_off_apwr(uint16 ring_position)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0105
    nc_slave_apwr_uint8(ring_position, SVR_REQ_CH_VALID_REG, 0x00, &flag);
    if (!flag)
        return false;

    return true;
}

// 3.服务请求通道写函数
// 站地址寻址
bool service_req_channel_fchwr(uint16 station_address, uint8 *data, uint16 len, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, FCHWR, station_address, offset, len, 0x00, data, 0x00);

    // 报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度
    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 7 + len));
    if ((irq & 0x03) != 0x01)
        return false;

    return true;
}

// 3.服务请求通道写函数
// 顺序寻址
bool service_req_channel_achwr(uint16 ring_position, uint8 *data, uint16 len, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, ACHWR, ring_position, offset, len, 0x00, data, 0x00);

    // 报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度
    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 7 + len));
    if ((irq & 0x03) != 0x01)
        return false;
    return true;
}

// 服务应答通道

// 0.2.通道1 一键配置
bool service_resp_channel_config_oneclick(uint16 ring_position)
{
    uint8 data[6];
    memset(data, 0, 6);
    reverse_array(data, 6);
    bool valid = true;
    nc_slave_aprd_all(ring_position, 0x42A, (uint8 *)&data, &valid, 6);
    print_data(data, 6);
    uint16 phy_addr = (uint16)data[0] | (uint16)data[1] << 8;
    uint16 ch_len = (uint16)data[2] | (uint16)data[3] << 8;
    uint8 control = (uint16)data[4];
    if (data[5] == 0x80)
    {
        service_req_channel_on_apwr(ring_position);
    }
    else if (data[5] == 0x00)
    {
        service_req_channel_off_apwr(ring_position);
    }
    else
    {
        printf("通道1启停配置区有错误\n");
    }
    service_resp_channel_config_apwr(ring_position, phy_addr, ch_len, control);
    return true;
}

// 4.通道1 服务应答通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool service_resp_channel_config_fpwr(uint16 station_address, uint16 phy_addr,
                                      uint16 len, uint8 control)
{
    bool flag; // 检验成功与否的标志位

    nc_slave_fpwr_uint16(station_address, SVR_RESP_CH_ADDR_REG, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint16(station_address, SVR_RESP_CH_LEN_REG, len, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint8(station_address, SVR_RESP_CH_INT_REG, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 4.通道1 服务应答通道 配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 顺序寻址
bool service_resp_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                      uint16 len, uint8 control)
{
    bool flag;

    nc_slave_apwr_uint16(ring_position, SVR_RESP_CH_ADDR_REG, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint16(ring_position, SVR_RESP_CH_LEN_REG, len, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint8(ring_position, SVR_RESP_CH_INT_REG, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 5.服务应答通道开关
// 站地址寻址
bool service_resp_channel_on_fpwr(uint16 station_address)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_fpwr_uint8(station_address, SVR_RESP_CH_VALID_REG, 0x80, &flag);

    if (!flag)
        return false;

    return true;
}

bool service_resp_channel_off_fpwr(uint16 station_address)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_fpwr_uint8(station_address, SVR_RESP_CH_VALID_REG, 0x00, &flag);

    if (!flag)
        return false;

    return true;
}

// 5.服务应答通道开关
// 顺序寻址
bool service_resp_channel_on_apwr(uint16 ring_position)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_apwr_uint8(ring_position, SVR_RESP_CH_VALID_REG, 0x80, &flag);
    if (!flag)
        return false;

    return true;
}

bool service_resp_channel_off_apwr(uint16 ring_position)
{
    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_apwr_uint8(ring_position, SVR_RESP_CH_VALID_REG, 0x00, &flag);
    if (!flag)
        return false;

    return true;
}

// 6.服务应答通道读函数
// 站地址寻址
bool service_resp_channel_fchrd(uint16 station_address, uint8 *data, uint16 len, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    // 清空读取的缓存区
    memset(data, 0, len);
    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, FCHRD, station_address, offset, len, 0x01, data, 0x00);

    // 报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度
    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 7 + len));
    if ((irq & 0x03) != 0x01)
        return false;

    memcpy(data, frame.dgrambuff + 7, len);
    return true;
}

// 6.服务应答通道读函数
// 顺序寻址
bool service_resp_channel_achrd(uint16 ring_position, uint8 *data, uint16 len, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    // 清空读取的缓存区
    memset(data, 0, len);
    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, ACHRD, ring_position, offset, len, 0x01, data, 0x00);

    // 报文添加报错，由于需要读取的字节超出 单报文/每帧 所允许的数据区的最大长度
    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
        return false;

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 7 + len));
    if ((irq & 0x03) != 0x01)
        return false;

    memcpy(data, frame.dgrambuff + 7, len);
    print_data(data, len);
    return true;
}

// 7.检测服务应答通道是否有数据，是否为满，是否可读
channel_status_t status_service_channel_respond_by_ring_position(uint16 ring_position)
{
    bool flag;
    uint8 status = nc_slave_aprd_uint8(ring_position, SVR_RESP_CH_STATUS_REG, &flag);
    /**the read status operation failed**/
    if (!flag)
        return UNDEFINE;

    if ((status & 0x30) >> 4 == 0x02)
        return FULL;
    else if ((status & 0x30) >> 4 == 0x01)
        return EMPTY;
    else
        return UNDEFINE;
}

// 读取服务应答通道的数据，将数据输出，data接收读出字节的首地址，由调用者保证data的区域有足够的空间接收读出的数据
bool service_channel_respond_by_ring_position(uint16 ring_position, uint16 len_ch, uint8 *data)
{
    int16 result;
    uint16 irq;
    nc_frame_t frame;
    uint8 ByteArray[len_ch];
    initframe(&frame);

    result = chAccessdgrmAdd(&frame, ACHRD, ring_position, 0x00, len_ch, SVR_RESP_CH, ByteArray,0);

    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        return false;
    }

    /**检测irq**/
    /**frame.dgrambuff + result 为本datagram起始地址，加7之后跳过datagram头部，加len_ch跳过数据区，指向IRQ**/
    irq = nctohs(*(uint16 *)(frame.dgrambuff + result + 7 + len_ch));
    if ((irq & 0x03) != 0x01)
        return false;

    /**跳过datagram的7个字节头部和2个字节表示长度的区域 应该没有跳过两个字节表示长度的区域**/
    memcpy(data, frame.dgrambuff + result + 7, len_ch);
    return true;
}

/*   NC_WRITE_U16(datagram, 0x06); // channel data length
    NC_WRITE_U8 (datagram + 2, 0x01); // CoN service
    NC_WRITE_U8 (datagram + 3, 0x00);
    NC_WRITE_U8 (datagram + 4, 0x01);
    NC_WRITE_U8 (datagram + 5, 0x01);
    NC_WRITE_U16(datagram + 6, 0x01);// deliver all SDOs!*/
//获取服务应答通道的数据
uint8 *nc_slave_res_channel_fetch(const nc_slave_t *slave, /**< slave */
                              uint8 *datagram, /**< datagram */
                             uint8 *type, /**< expected mailbox protocol */
                             uint16 *size /**< size of the received data */
                             )
{
    uint16 data_size;

    data_size = NC_READ_U16(datagram);

 /*   if (data_size + EC_MBOX_HEADER_SIZE > slave->configured_tx_mailbox_size) {
        EC_SLAVE_ERR(slave, "Corrupt mailbox response received!\n");
        ec_print_data(datagram->Data, slave->configured_tx_mailbox_size);
        return ERR_PTR(-EPROTO);
    } */

    *type = NC_READ_U8(datagram + 2);
    *size = data_size;
    return datagram+4;
}