#include "process_data_channel.h"
#include "datagram.h"
#include <string.h>
#include "format.h"
#include "reverse_array.h"

// 1.通道配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 站地址寻址
bool proc_data_channel_config(uint16 station_address, uint16 phy_addr,
                              uint16 len, uint8 control, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag; // 检验成功与否的标志位

    nc_slave_fpwr_uint16(station_address, REG_BASE_PRO_CHANNEL_PHY_ADDRESS + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint16(station_address, REG_BASE_PRO_CHANNEL_LEN + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, len, &flag);
    if (!flag)
        return false;

    nc_slave_fpwr_uint8(station_address, REG_BASE_PRO_CHANNEL_CONTROL + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 1.通道配置：站地址、通道起始物理地址，通道长度，控制字，通道号
// 顺序寻址
bool proc_data_channel_config_apwr(uint16 ring_position, uint16 phy_addr,
                                   uint16 len, uint8 control, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag;

    nc_slave_apwr_uint16(ring_position, REG_BASE_PRO_CHANNEL_PHY_ADDRESS + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, phy_addr, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint16(ring_position, REG_BASE_PRO_CHANNEL_LEN + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, len, &flag);
    if (!flag)
        return false;

    nc_slave_apwr_uint8(ring_position, REG_BASE_PRO_CHANNEL_CONTROL + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, control, &flag);
    if (!flag)
        return false;

    return true;
}

// 2.过程数据通道开关
// 站地址寻址
bool proc_data_channel_on(uint16 station_address, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_fpwr_uint8(station_address, REG_BASE_PRO_CHANNEL_SWITCH + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, 0x80, &flag);

    if (!flag)
        return false;

    return true;
}

bool proc_data_channel_off(uint16 station_address, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_fpwr_uint8(station_address, REG_BASE_PRO_CHANNEL_SWITCH + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, 0x00, &flag);

    if (!flag)
        return false;

    return true;
}

// 2.过程数据通道开关
// 顺序寻址
bool proc_data_channel_on_apwr(uint16 ring_position, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_apwr_uint8(ring_position, REG_BASE_PRO_CHANNEL_SWITCH + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, 0x80, &flag);
    if (!flag)
        return false;

    return true;
}

bool proc_data_channel_off_apwr(uint16 ring_position, uint8 ch_No)
{
    // 通道号不正确，越出范围
    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    bool flag;

    // 写入 0x80 启动过程数据通道，参见寄存器手册 reg 0x0125
    nc_slave_apwr_uint8(ring_position, REG_BASE_PRO_CHANNEL_SWITCH + (ch_No - MINI_PRO_CHANNEL_NO) * PRO_CHANNEL_CONFIG_SIZE, 0x00, &flag);
    if (!flag)
        return false;

    return true;
}

// 3.通道读写函数
// 站地址寻址
bool proc_data_channel_fchrd(uint16 station_address, uint8 *data, uint16 len, uint8 ch_No, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    // 清空读取的缓存区
    memset(data, 0, len);
    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, FCHRD, station_address, offset, len, ch_No, data, 0x00);

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
    print_data(data,len);
    return true;
}

bool proc_data_channel_fchwr(uint16 station_address, uint8 *data, uint16 len, uint8 ch_No, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, FCHWR, station_address, offset, len, ch_No, data, 0x00);

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

// 3.通道读写函数
// 顺序寻址
bool proc_data_channel_achrd(uint16 ring_position, uint8 *data, uint16 len, uint8 ch_No, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    // 清空读取的缓存区
    memset(data, 0, len);
    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, ACHRD, ring_position, offset, len, ch_No, data, 0x00);

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
    print_data(data,len);
    return true;
}

bool proc_data_channel_achwr(uint16 ring_position, uint8 *data, uint16 len, uint8 ch_No, uint16 offset)
{
    nc_frame_t frame;
    uint16 irq = 0x00;
    int16 result;

    if (ch_No > MAX_PRO_CHANNEL_NO || ch_No < MINI_PRO_CHANNEL_NO)
    {
        printf("out of the range of process data channel.\n");
        return false;
    }

    initframe(&frame);

    // 站内偏移地址设为0x00,irq设为0x00
    result = chAccessdgrmAdd(&frame, ACHWR, ring_position, offset, len, ch_No, data, 0x00);

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