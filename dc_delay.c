#include "dc_delay.h"

//计算从参考从站到各从站的传输延时delay（不包含N2CC内的片内延时），并将其写入寄存器0x0400

void dcCaldelay(uint8 slave_num)
{

    int i,j=0;
    bool flag;
    nc_frame_t frame;
    uint32 delay[4] = {0};
    uint32 delay_total[4] = {0};
    uint32 delay_in_total[4] = {0};

    for(j=0; j<100; j++)
    {
        initframe(&frame);
        timesdgrmAdd(&frame);
        FrameTransmit(&frame);
        FrameReceive(&frame);

        uint32 slave_timestampRX0[4];
        uint32 slave_timestampRX1[4];
        uint8 slave_timestampTX0[4];
        uint8 slave_timestampTX1[4];
        for(i=0; i<slave_num; i++)
        {
            slave_timestampRX0[i] = nc_slave_aprd_uint32(i,0x0030,&flag);
            slave_timestampRX1[i] = nc_slave_aprd_uint32(i,0x0034,&flag);
            slave_timestampTX0[i] = nc_slave_aprd_uint8(i,0x003C,&flag);
            slave_timestampTX1[i] = nc_slave_aprd_uint8(i,0x003D,&flag);
        }

        for(i=0; i<slave_num-1; i++)
        {
            if(i == 0)
            {
                delay_in_total[i] = slave_timestampTX1[0];
                continue;
            }
            delay_in_total[i] = delay_in_total[i-1] + slave_timestampTX0[i] + slave_timestampTX1[i];
        }

        for(i=1; i<slave_num; i++)
        {
            if(i == (slave_num-1))
            {
                delay[i] = (slave_timestampRX1[0] - (slave_timestampRX0[0]) - (8*slave_timestampTX0[i]) - (8*delay_in_total[i-1]))/2;
                delay_total[i] += delay[i];
            }
            else
            {
                delay[i] = (slave_timestampRX0[i] - slave_timestampRX0[0] + slave_timestampRX1[0]\
                        - (slave_timestampRX1[i] + 8*slave_timestampTX0[i]) - (8*delay_in_total[i-1]))/2;
                delay_total[i] += delay[i];
            }
        }
    }

    delay[0] = 0;
    for(i=0; i<slave_num; i++)
    {
        delay_total[i] = delay_total[i]/100;
        nc_slave_apwr_uint32(i,0x0040,delay_total[i],&flag);
        printf("delay[%d] = %d\n",i,delay_total[i]);
    }

    


}