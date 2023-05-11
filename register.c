#include <string.h>
#include "format.h"
#include "register.h"
#include "datagram.h"
#include "reverse_array.h"
/**单字节从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint8 nc_slave_aprd_uint8(uint16 ring_position, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint8 rd_byte = 0x00;
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, APRD, ring_position,
                     offset, sizeof(uint8), &rd_byte, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 8));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return *(frame.dgrambuff + 7);
}

/**单字节从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint8 nc_slave_apwr_uint8(uint16 ring_position, uint16 offset, uint8 byte, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, APWR, ring_position,
                     offset, sizeof(uint8), &byte, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 8));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return byte;
}

/**16位 从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint16 nc_slave_aprd_uint16(uint16 ring_position, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint16 rd_byte = 0x00;
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, APRD, ring_position,
                     offset, sizeof(uint16), (uint8 *)&rd_byte, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 9));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return nctohs(*(uint16 *)(frame.dgrambuff + 7));
}

/**16位 从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint16 nc_slave_apwr_uint16(uint16 ring_position, uint16 offset,
                            uint16 word, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;
    uint16 word_wr = htoncs(word);
    initframe(&frame);
    regAccessdgrmAdd(&frame, APWR, ring_position,
                     offset, sizeof(uint16), (uint8 *)&word_wr, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 9));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return word;
}

/**32位 从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint32 nc_slave_aprd_uint32(uint16 ring_position, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint32 rd_byte = 0x00;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, APRD, ring_position,
                     offset, sizeof(uint32), (uint8 *)&rd_byte, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 11));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return nctohl(*(uint32 *)(frame.dgrambuff + 7));
}

/**32位 从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint32 nc_slave_apwr_uint32(uint16 ring_position, uint16 offset,
                            uint32 Dword, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;
    uint32 Dword_wr = htoncl(Dword);

    initframe(&frame);
    regAccessdgrmAdd(&frame, APWR, ring_position,
                     offset, sizeof(uint32), (uint8 *)&Dword_wr, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 11));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return Dword;
}

/**专为通道配置而写的6byte读，读成功 则 *valid的值为真，否则为假**/

/**32位 从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint8 *nc_slave_aprd_6byte(uint16 ring_position, uint16 offset,
                           uint8 *data, bool *valid)
{
    nc_frame_t frame;
    uint8 rd_byte[6] = {0, 0, 0, 0, 0, 0};
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, APRD, ring_position,
                     offset, 6, rd_byte, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 13));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;

    for (size_t i = 0; i < 6; i++)
    {
        *(data + i) = *(frame.dgrambuff + 7 + i);
    }

    return data;
}

// 任意字节 寄存器 读 顺序寻址
uint8 *nc_slave_aprd_all(uint16 ring_position, uint16 offset,
                         uint8 *data, bool *valid, uint16 len)
{
    nc_frame_t frame;
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, APRD, ring_position,
                     offset, len, data, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 7 + len));

    memcpy(data, frame.dgrambuff + 7, len);

    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return data;
}

/**专为通道配置而写的6byte写，写成功 则 *valid的值为真，否则为假**/

uint8 *nc_slave_apwr_6byte(uint16 ring_position, uint16 offset,
                           uint8 *data, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, APWR, ring_position,
                     offset, 6, data, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 13));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        // return 0;
    }
    *valid = true;
    return data;
}

/**单字节从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint8 nc_slave_fprd_uint8(uint16 station_address, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint8 rd_byte = 0x00;
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, FPRD, station_address,
                     offset, sizeof(uint8), &rd_byte, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 8));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return *(frame.dgrambuff + 7);
}

/**单字节从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint8 nc_slave_fpwr_uint8(uint16 station_address, uint16 offset, uint8 byte, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, FPWR, station_address,
                     offset, sizeof(uint8), &byte, 0);
    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 8));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return byte;
}

/**16位 从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint16 nc_slave_fprd_uint16(uint16 station_address, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint16 rd_byte = 0x00;
    uint16 irq;
    initframe(&frame);
    regAccessdgrmAdd(&frame, FPRD, station_address,
                     offset, sizeof(uint16), (uint8 *)&rd_byte, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 9));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return nctohs(*(uint16 *)(frame.dgrambuff + 7));
}

/**16位 从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint16 nc_slave_fpwr_uint16(uint16 station_address, uint16 offset,
                            uint16 word, bool *valid)
{
    nc_frame_t frame;              // 创建报文帧
    uint16 irq;                    // 创建IRQ
    uint16 word_wr = htoncs(word); //
    initframe(&frame);             // 初始化报文帧
    regAccessdgrmAdd(&frame, FPWR, station_address,
                     offset, sizeof(uint16), (uint8 *)&word_wr, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 9));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return word;
}

/**32位 从站读寄存器，读成功 则 *valid的值为真，否则为假**/
uint32 nc_slave_fprd_uint32(uint16 station_address, uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint32 rd_byte = 0x00;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, FPRD, station_address,
                     offset, sizeof(uint32), (uint8 *)&rd_byte, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 11));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return nctohl(*(uint32 *)(frame.dgrambuff + 7));
}

/**32位 从站写寄存器，写成功 则 *valid的值为真，否则为假**/
uint32 nc_slave_fpwr_uint32(uint16 station_address, uint16 offset,
                            uint32 Dword, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;
    uint32 Dword_wr = htoncl(Dword);
    initframe(&frame);
    regAccessdgrmAdd(&frame, FPWR, station_address,
                     offset, sizeof(uint32), (uint8 *)&Dword_wr, 0);

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 11));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }
    *valid = true;
    return Dword;
}

/*****广播方式读写寄存器******/
uint8 nc_slave_brd_uint8(uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint8 rd_byte = 0x00;

    initframe(&frame);
    regAccessdgrmAdd(&frame, BRD, 0x0000,
                     offset, sizeof(uint8), &rd_byte, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    /***广播读 不检查 IRQ***/

    *valid = true;
    return *(frame.dgrambuff + 7);
}

uint8 nc_slave_bwr_uint8(uint16 offset, uint8 byte, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;

    initframe(&frame);
    regAccessdgrmAdd(&frame, BWR, 0x0000,
                     offset, sizeof(uint8), &byte, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 8));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }

    *valid = true;
    return *(frame.dgrambuff + 7);
}

uint16 nc_slave_brd_uint16(uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint16 rd_byte = 0x00;

    initframe(&frame);
    regAccessdgrmAdd(&frame, BRD, 0x0000,
                     offset, sizeof(uint16), (uint8 *)&rd_byte, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    /***广播读 不检查 IRQ***/

    *valid = true;
    return nctohs(*(uint16 *)(frame.dgrambuff + 7));
}

uint16 nc_slave_bwr_uint16(uint16 offset, uint16 word, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;
    uint16 word_wr = htoncs(word);
    initframe(&frame);
    /***广播读写，站地址无意义，写为0***/
    regAccessdgrmAdd(&frame, BWR, 0x0000,
                     offset, sizeof(uint16), (uint8 *)&word_wr, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 9));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }

    *valid = true;
    return word;
}

uint32 nc_slave_brd_uint32(uint16 offset, bool *valid)
{
    nc_frame_t frame;
    uint32 rd_byte = 0x00;

    initframe(&frame);
    regAccessdgrmAdd(&frame, BRD, 0x0000,
                     offset, sizeof(uint32), (uint8 *)&rd_byte, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    /***广播读 不检查 IRQ***/

    *valid = true;
    return nctohl(*(uint16 *)(frame.dgrambuff + 7));
}

uint32 nc_slave_bwr_uint32(uint16 offset, uint32 Dword, bool *valid)
{
    nc_frame_t frame;
    uint16 irq;
    uint32 Dword_wr = htoncl(Dword);
    initframe(&frame);
    /***广播读写，站地址无意义，写为0***/
    regAccessdgrmAdd(&frame, BWR, 0x0000,
                     offset, sizeof(uint32), (uint8 *)&Dword_wr, 0);
    FrameTransmit(&frame);

    if (FrameReceive(&frame) < 0)
    {
        *valid = false;
        return 0;
    }

    irq = nctohs(*(uint16 *)(frame.dgrambuff + 11));
    if ((irq & 0x03) != 0x01)
    {
        *valid = false;
        return 0;
    }

    *valid = true;
    return Dword;
}
