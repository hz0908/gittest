
#include "slave.h"
#include "format.h"
#include "frame.h"
#include "datagram.h"
#include "string.h"
#include "register.h"

bool station_address_config(uint16 station_address,uint16 ring_position)
{
    bool flag;
    nc_slave_apwr_uint16(ring_position,REG_STATION_ADDR,station_address,&flag);
    if(!flag)
        return false;
    else
        return true;
}

bool slave_info_acquire(uint16 station_address,nc_info_t *info)
{
    uint16 vendor_id,product_code,version,revision;
    uint8 dc_num,pdc_num,sdc_num,fifo_num,map_unit_num,ram_size;

    bool flag;

    if(!info)
        return false;

    vendor_id = nc_slave_fprd_uint16(station_address,REG_VENDOR_ID,&flag);
    if(!flag)
        return false;
    product_code = nc_slave_fprd_uint16(station_address,REG_PRODUCT_CODE,&flag);
    if(!flag)
        return false;
    version = nc_slave_fprd_uint16(station_address,REG_VERSION,&flag);
    if(!flag)
        return false;
    revision = nc_slave_fprd_uint16(station_address,REG_REVISION,&flag);
    if(!flag)
        return false;
    dc_num = nc_slave_fprd_uint8(station_address,REG_DCLK_NUM,&flag);
    if(!flag)
        return false;
    pdc_num = nc_slave_fprd_uint8(station_address,REG_PROC_CHANNEL_NUM,&flag);
    if(!flag)
        return false;
    sdc_num = nc_slave_fprd_uint8(station_address,REG_SVR_CHANNEL_NUM,&flag);
    if(!flag)
        return false;
    fifo_num = nc_slave_fprd_uint8(station_address,REG_FIFO_NUM,&flag);
    if(!flag)
        return false;
    map_unit_num = nc_slave_fprd_uint8(station_address,REG_MAP_UNIT_NUM,&flag);
    if(!flag)
        return false;
    ram_size = nc_slave_fprd_uint8(station_address,REG_RAM_SIZE,&flag);
    if(!flag)
        return false;

    info->vendor = vendor_id;
    info->product_code = product_code;
    info->version = version;
    info->revision = revision;
    info->dclksupport = dc_num;
    info->pdcsupport = pdc_num;
    info->sdcsupport = sdc_num;
    info->fifosupport = fifo_num;
    info->mapsupport = map_unit_num;
    info->ramsize = ram_size;

    return true;
}
