#include "channel_view.h"
#include "reverse_array.h"
#include "register.h"
#include <stdio.h>
#include <string.h>

// 查看配置信息
struct ch_config_inf channel_view(uint16 ring_position, uint8 ch_No)
{

    uint16 len;
    if (ch_No == 0 || ch_No == 1)
    {
        len = 16;
    }
    else
    {
        len = 14;
    }
    uint8 data[len];
    memset(data, 0, len);
    reverse_array(data, len);
    bool valid = true;
    nc_slave_aprd_all(ring_position, 0x0100 + ch_No * 0x0010, (uint8 *)&data, &valid, len);
    print_data(data, len);
    struct ch_config_inf cci;
    cci.ch_start_addr = (uint16)data[0] | (uint16)data[1] << 8;
    cci.ch_len = (uint16)data[2] | (uint16)data[3] << 8;
    cci.ch_interrrupt_switch = (uint8)data[4];
    cci.ch_switch = (uint8)data[5];
    cci.ch_status = (uint8)data[7];
    cci.ch_M_offset = (uint16)data[8] | (uint16)data[9] << 8;
    cci.ch_S_addr = (uint16)data[10] | (uint16)data[11] << 8;
    cci.ch_M_Bytes = (uint16)data[12] | (uint16)data[13] << 8;

    printf("通道%d信息:\n", ch_No);
    printf("01.ch_start_addr:0x%02x\n", cci.ch_start_addr);
    printf("02.ch_len:0x%02x\n", cci.ch_len);
    printf("03.ch_interrrupt_switch:0x%x\t", cci.ch_interrrupt_switch);
    
    if ((ch_No != 0) & (ch_No != 1))
    {

        switch ((cci.ch_interrrupt_switch & 0b10000) >> 4)
        {
        case 0:
            printf("主站写从站读方向\t");
            break;
        default:
            printf("从站写主站读方向\t");
        };
    }

    switch (cci.ch_interrrupt_switch & 0b11)
    {
    case 0:
        printf("M中断未允许,S中断未允许\n");
        break;
    case 1:
        printf("M中断允许,S中断未允许\n");
        break;
    case 2:
        printf("M中断未允许,S中断允许\n");
        break;
    case 3:
        printf("M中断允许,S中断允许\n");
        break;
    default:
        printf("中断允许区出现错误\n");
    };

    printf("04.ch_switch:0x%x\t", cci.ch_switch);

    switch (cci.ch_switch)
    {
    case 0x00:
        printf("该通道未开启\n");
        break;
    case 0x80:
        printf("该通道开启\n");
        break;
    default:
        printf("通道启停区出现错误\n");
    };

    printf("05.ch_status:0x%x\t", cci.ch_status);

    if (cci.ch_status & 0b00000001)
    {
        printf("主站中断请求\t");
    }
    else
    {
        printf("主站未中断请求\t");
    }

    if ((cci.ch_status & 0b00000010) >> 1)
    {
        printf("从站中断请求\t");
    }
    else
    {
        printf("从站未中断请求\t");
    }

    if ((cci.ch_status & 0b00010000) >> 4)
    {
        printf("通道空状态\t");
    }
    else
    {
        printf("通道不是空状态\t");
    }

    if ((cci.ch_status & 0b00100000) >> 5)
    {
        printf("通道满状态\n");
    }
    else
    {
        printf("通道不是满状态\n");
    }

    printf("06.ch_M_offset:0x%02x\t", cci.ch_M_offset);

    if (cci.ch_M_offset == 0xffff)
    {
        printf("主站不可读写\n");
    }
    else
    {
        printf("\n");
    }

    printf("07.ch_S_addr:0x%02x\t", cci.ch_S_addr);

    if (cci.ch_S_addr == 0x0000)
    {
        printf("从站不可读写\n");
    }
    else
    {
        printf("\n");
    }

    if (ch_No == 0 || ch_No == 1)
    {
        cci.ch_S_Bytes = (uint16)data[14] | (uint16)data[15] << 8;
        printf("08.ch_M_Bytes:0x%02x\n", cci.ch_M_Bytes);
        printf("09.ch_S_Bytes:0x%02x\n", cci.ch_S_Bytes);
    }

    printf("\n");
    return cci;
}