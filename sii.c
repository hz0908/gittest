#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include <QDebug>
#include "format.h"
#include "slave.h"
#include "frame.h"
#include "datagram.h"
#include "register.h"
#include "sdo.h"

// Slave scan state: SII DATA
static uint8 last_pdo_mapping_ID = 1;
static uint16 in_mapping_end_address, out_mapping_end_address;

// 应用信息区解析
void sii_fetch(nc_slave_t *slave)
{
    // 接收应用信息区数据
    uint8 *data = (uint8 *)malloc(0x400 * sizeof(uint8));
    memset(data, 0, 0x400 * sizeof(uint8));
    bool valid = true;
    data = *nc_slave_aprd_all(slave->ring_position, 0x400, data, &valid, 0x400);

    // 分成固定内容和可变内容
    uint8 *datagram = (uint8 *)malloc(0x40 * sizeof(uint8));
    uint8 *cat_datagram = (uint8 *)malloc((0x400 - 0x40) * sizeof(uint8));
    memcpy(datagram, data, 0x40 * sizeof(uint8));
    memcpy(cat_datagram, data, (0x400 - 0x40) * sizeof(uint8));

    // 解析固定内容
    basic_slave_info(slave, datagram);
    printf("slave->Vendor_ID = %x\nslave->Product_ID = %x\n", slave->basic_info->Vendor_ID, slave->basic_info->Product_ID);
    // qDebug("slave->Vendor_ID = %x\n slave->Product_ID = %x\n", slave->basic_info->Vendor_ID, slave->basic_info->Product_ID);

    // 解析固定内容
    slave->pdo_channel_count = 0;
    slave->service_channel_count = 0;
    slave->string_count = 0;
    uint16 cat_type, cat_size = 0;
    cat_type = NC_READ_U16(cat_datagram);
    cat_size = NC_READ_U16(cat_datagram + 2);

    while (cat_type != 0xFFFF) // not the last category
    {
        if (cat_type > 0x0000 && cat_type < 0x1000)
        {
            cat_datagram = nc_slave_fetch_strings(slave, cat_datagram);
        }
        else if (cat_type > 0x1001 && cat_type < 0x1018)
        {
            cat_datagram = nc_slave_fetch_channel(slave, cat_datagram);
        }
        else if (cat_type > 0x1BFF && cat_type < 0x1C20)
        {
            cat_datagram = nc_slave_fetch_pdo_mapping(slave, cat_datagram);
        }
        else if (cat_type > 0x1C1F && cat_type < 0x1C40)
        {
            cat_datagram = nc_slave_fetch_entry_info(slave, cat_datagram);
        }
        cat_type = NC_READ_U16(cat_datagram);
        cat_size = NC_READ_U16(cat_datagram + 2);
    }
}

// 解析固定内容
void basic_slave_info(nc_slave_t *slave, uint8 *datagram)
{
    /*
        slave_basic_info_t *basicinfo = (slave_basic_info_t*)malloc(sizeof(slave_basic_info_t));
        basicinfo->Vendor_ID = nctohl(*(uint32 *)(datagram + 7));
        basicinfo->Product_ID = nctohl(*(uint32 *)(datagram + 11));
        basicinfo->Revision_Num = nctohl(*(uint32 *)(datagram + 15));
        basicinfo->Serial_Number = nctohl(*(uint32 *)(datagram + 19));
        basicinfo->Channel_Protocol = nctohl(*(uint32 *)(datagram + 39));
        basicinfo->Service_req_channel_start_address = nctohs(*(uint16 *)(datagram + 43));
        basicinfo->Service_req_channel_length= nctohs(*(uint16 *)(datagram + 45));
        basicinfo->Service_req_channel_IRQ_Enable= *(datagram+ 47);
        basicinfo->Service_req_channel_Enable= *(datagram+ 48);
        basicinfo->Service_ans_channel_start_address = nctohs(*(uint16 *)(datagram + 49));
        basicinfo->Service_ans_channel_length= nctohs(*(uint16 *)(datagram + 51));
        basicinfo->Service_ans_channel_IRQ_Enable= *(datagram+ 53);
        basicinfo->Service_ans_channel_Enable= *(datagram+ 54);
        basicinfo->Sync_Supported  = *(datagram+ 55);
        basicinfo->Sync_Pulse_Width = nctohs(*(uint16 *)(datagram + 57));
        basicinfo->Minimum_Cycle_Time = nctohl(*(uint32 *)(datagram + 59));
        basicinfo->Latch_Signal_Supported = *(datagram+ 63);
        basicinfo->Slave_Watchdog_Supported = *(datagram+ 67);
    */
    slave_basic_info_t *basicinfo = (slave_basic_info_t *)malloc(sizeof(slave_basic_info_t));
    basicinfo->Vendor_ID = NC_READ_U32(datagram);
    basicinfo->Product_ID = NC_READ_U32(datagram + 4);
    basicinfo->Revision_Num = NC_READ_U32(datagram + 8);
    basicinfo->Serial_Number = NC_READ_U32(datagram + 0x0c);
    basicinfo->DeviceType = NC_READ_U32(datagram + 0x10);
    basicinfo->Node_ID = NC_READ_U16(datagram + 0x14);
    basicinfo->Channel_Protocol = NC_READ_U32(datagram + 0x20);
    basicinfo->Service_req_channel_start_address = NC_READ_U16(datagram + 0x24);
    basicinfo->Service_req_channel_length = NC_READ_U16(datagram + 0x26);
    basicinfo->Service_req_channel_IRQ_Enable = NC_READ_U8(datagram + 0x28);
    basicinfo->Service_req_channel_Enable = NC_READ_U8(datagram + 0x29);
    basicinfo->Service_res_channel_start_address = NC_READ_U16(datagram + 0x2a);
    basicinfo->Service_res_channel_length = NC_READ_U16(datagram + 0x2c);
    basicinfo->Service_res_channel_IRQ_Enable = NC_READ_U8(datagram + 0x2e);
    basicinfo->Service_res_channel_Enable = NC_READ_U8(datagram + 0x2f);
    basicinfo->Sync_Supported = NC_READ_U8(datagram + 0x30);
    basicinfo->Sync_Pulse_Width = NC_READ_U16(datagram + 0x32);
    basicinfo->Minimum_Cycle_Time = NC_READ_U32(datagram + 0x34);
    basicinfo->Latch_Signal_Supported = NC_READ_U8(datagram + 0x38);
    basicinfo->Slave_Watchdog_Supported = NC_READ_U8(datagram + 0x3c);
    slave->basic_info = basicinfo;
}

uint8 *nc_slave_fetch_strings(nc_slave_t *slave, uint8 *cat_datagram)
{
    uint16 cat_type, cat_size = 0;
    cat_type = NC_READ_U16(cat_datagram);
    cat_size = NC_READ_U16(cat_datagram + 2);
    while (cat_type < 0x0010)
    {
        switch (cat_type)
        {
        case 0x0001:
            if (cat_size)
            {
                slave->Vendor_Name = (char *)malloc(sizeof(char) * (cat_size + 1));
                memcpy(slave->Vendor_Name, cat_datagram + 4, cat_size);
                slave->Vendor_Name[cat_size] = 0;
            }
            printf("Vendor_Name : %s\n", slave->Vendor_Name);
            break;
        case 0x0002:
            if (cat_size)
            {
                slave->Device_Name = (char *)malloc(sizeof(char) * (cat_size + 1));
                memcpy(slave->Device_Name, cat_datagram + 4, cat_size);
                slave->Device_Name[cat_size] = 0;
            }
            printf("Device_Name : %s\n", slave->Device_Name);
            break;
        case 0x0003:
            if (cat_size)
            {
                slave->Latch_Event_Name = (char *)malloc(sizeof(char) * (cat_size + 1));
                memcpy(slave->Latch_Event_Name, cat_datagram + 4, cat_size);
                slave->Latch_Event_Name[cat_size] = 0;
            }
            printf("Latch_Event_Name : %s\n", slave->Latch_Event_Name);
            break;
        default:
            if (0x0009 < cat_type < 0x1000)
            {
                slave->string_count++;
                if (slave->string_count = 1)
                {
                    if (!(slave->strings = (char **)malloc(sizeof(char *) * slave->string_count)))
                    {
                        printf("Failed to allocate string array memory.\n");
                    }
                }
                else
                {
                    if (!(slave->strings = (char **)realloc(slave->strings, sizeof(char *) * slave->string_count)))
                    {
                        printf("Failed to allocate string array memory.\n");
                    }
                }
                // allocate memory for string structure and data at a single blow
                if (!(slave->strings[slave->string_count - 1] = (char *)malloc(sizeof(char) * (cat_size + 1))))
                {
                    printf("Failed to allocate string memory.\n");
                }
                memcpy(slave->strings[slave->string_count - 1], cat_datagram + 4, cat_size);
                slave->strings[slave->string_count - 1][cat_size] = 0x00; // append binary zero
                printf("string %d : %s\n", slave->string_count, slave->strings[slave->string_count - 1]);
            }
            break;
        }
    }
    cat_datagram = cat_datagram + 4 + cat_size;
    return cat_datagram;
}

uint8 *nc_slave_fetch_channel(nc_slave_t *slave, uint8 *cat_datagram)
{
    // uint8 number_pdo = slave->pdo_channel_count;
    // uint8 number_serv = slave->service_channel_count;
    pdo_channel_t *channel;

    // 查看是service_channel还是pdo_channel
    uint32 channel_protocol = 0;
    channel_protocol = NC_READ_U32(cat_datagram + 4);
    if (channel_protocol)
    {
        slave->service_channel_count++;
    }
    else
    {
        slave->pdo_channel_count++;
    }

    if (slave->pdo_channel_count = 1)
    {
        if (!(slave->pdo_channel_list = (pdo_channel_t *)malloc(sizeof(pdo_channel_t) * slave->pdo_channel_count)))
        {
            printf("Failed to allocate pdo_channel_list memory.\n");
        }
    }
    else
    {
        if (!(slave->pdo_channel_list = (pdo_channel_t *)realloc(slave->pdo_channel_list, sizeof(pdo_channel_t) * slave->pdo_channel_count)))
        {
            printf("Failed to allocate pdo_channel_list memory.\n");
        }
    }

    channel = &(slave->pdo_channel_list[slave->pdo_channel_count - 1]);
    channel->category_ID = NC_READ_U16(cat_datagram);
    channel->pdo_mapping = (pdo_object_mapping_t *)malloc(sizeof(pdo_object_mapping_t));
    channel->pdo_mapping->pdo_mapping_category_ID = NC_READ_U16(cat_datagram + 8);
    channel->channel_start_address = NC_READ_U16(cat_datagram + 10);
    channel->channel_length = NC_READ_U16(cat_datagram + 12);
    channel->IRQ_enable = NC_READ_U8(cat_datagram + 14);
    channel->channel_enable = NC_READ_U8(cat_datagram + 15);

    // if ((channel->IRQ_enable & 0x10) == 0x10)
    // {
    //     channel->pdo_mapping_ID = last_pdo_mapping_ID;
    //     channel->mapping_start_address = in_mapping_end_address;
    //     in_mapping_end_address = in_mapping_end_address + channel->channel_length;
    // }
    // else if ((channel->IRQ_enable & 0x10) == 0x00)
    // {
    //     channel->pdo_mapping_ID = last_pdo_mapping_ID + 1;
    //     channel->mapping_start_address = out_mapping_end_address;
    //     out_mapping_end_address = out_mapping_end_address + channel->channel_length;
    // }

    //                if( (last_mapping_end_address + channel->channel_length) >= 1480 )
    //                {
    //                    channel->pdo_mapping_ID = last_pdo_mapping_ID + 1;
    //                    last_pdo_mapping_ID++;
    //                    channel->mapping_start_address = 0;
    //                    last_mapping_end_address = channel->channel_length;
    //                }
    //                else
    //                {
    //                    channel->pdo_mapping_ID = last_pdo_mapping_ID;
    //                    channel->mapping_start_address = last_mapping_end_address;
    //                    last_mapping_end_address = last_mapping_end_address + channel->channel_length;
    //                }
    printf("pdo %d %#x mapping : %#x\n", slave->pdo_channel_count, channel->category_ID, channel->pdo_mapping->pdo_mapping_category_ID);
    cat_datagram = cat_datagram + 16;
}

uint8 *nc_slave_fetch_pdo_mapping(nc_slave_t *slave, uint8 *cat_datagram)
{

    //    uint8 pdo_number = slave->pdo_channel_count;
    pdo_entries_t *entry;
    pdo_channel_t *channel;
    int j, k = 0;
    uint16 category_ID;
    uint16 entry_number;
    uint8 channel_number;
    category_ID = NC_READ_U16(cat_datagram);
    entry_number = (NC_READ_U16(cat_datagram + 2)) / 2;
    channel_number = slave->pdo_channel_count;
    for (j = 0; j < channel_number; j++)
    {
        channel = &(slave->pdo_channel_list[j]);
        if (channel->pdo_mapping->pdo_mapping_category_ID == category_ID)
        {
            channel->pdo_mapping->pdo_entry = (pdo_entries_t *)malloc(sizeof(pdo_entries_t) * entry_number);
            channel->pdo_mapping->number_of_Entries = entry_number;
            for (k = 0; k < entry_number; k++)
            {
                entry = &(channel->pdo_mapping->pdo_entry[k]);
                entry->index = NC_READ_U16(cat_datagram + 2 * k + 4);
                // entry->index = (uint16)(NC_READ_U32(cat_datagram + 2 * k + 4));
                // entry->max_subindex = (uint8)(uint16)(NC_READ_U32(cat_datagram + 2*k +4)>>16);
            }
            cat_datagram = cat_datagram + 2 * entry_number + 4;
            return cat_datagram;
        }
    }
    printf("Warning:can't find the right PDO channel!\n");
    // qDebug("can't find the right PDO channel");
    return cat_datagram;
}
uint8 *nc_slave_fetch_entry_info(nc_slave_t *slave, uint8 *cat_datagram)
{
    pdo_channel_t *channel;
    pdo_entries_t *entry;
    uint16 entry_number, string_ID;
    int i, j;
    uint16 cat_ID, pdo_ID;
    uint8 channel_number;
    cat_ID = NC_READ_U16(cat_datagram);
    pdo_ID = cat_ID - 32;
    channel_number = slave->pdo_channel_count;
    for (i = 0; i < channel_number; i++)
    {
        channel = &(slave->pdo_channel_list[i]);
        if (channel->pdo_mapping->pdo_mapping_category_ID == pdo_ID)
        {
            entry_number = channel->pdo_mapping->number_of_Entries;
            for (j = 0; j < entry_number; j++)
            {
                entry = &(channel->pdo_mapping->pdo_entry[j]);
                string_ID = NC_READ_U16(cat_datagram + 2 * j + 4);
                entry->name = slave->strings[string_ID - 16];
            }
            cat_datagram = cat_datagram + entry_number * 2 + 4;
            return cat_datagram;
        }
    }
    printf("Warning:can't find the right pdo_channel!\n");
    return cat_datagram;
}

// sdo服务前需要初始化
void init_slave_basic_info(nc_slave_t *slave)
{
    slave_basic_info_t *basicinfo = (slave_basic_info_t *)malloc(sizeof(slave_basic_info_t));
    basicinfo->Service_req_channel_length = 0x80;
    basicinfo->Service_res_channel_length = 0x80;
    basicinfo->Service_req_channel_start_address = 0x800;
    basicinfo->Service_res_channel_start_address = 0x880;
    basicinfo->Channel_Protocol = 1;
    slave->basic_info = basicinfo;
    slave->ring_position = 0;
}

void pdo_entry_parameters_set(nc_slave_t *slave)
{
    uint8 datalen;
    uint8 maxsubindex = 0;
    uint32 pdodata = 0;

    for (int j = 0; j < slave->pdo_channel_count; j++)
    {
        for (int k = 0; k < slave->pdo_channel_list[j].pdo_mapping->number_of_Entries; k++)
        {
            SDO_Upload(slave, &maxsubindex,
                       (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->index, 0, 1);

            (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->max_subindex = maxsubindex; // 不包含0号索引
            (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_index = (uint16 *)malloc(sizeof(uint16) * maxsubindex);
            (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_subindex = (uint8 *)malloc(sizeof(uint8) * maxsubindex);
            (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_data_offset = (uint16 *)malloc(sizeof(uint16) * maxsubindex);
            // 上边这一行是我在结构体中新加的一个成员pdo_data_offset，表征每个pdodata在自己所在的pdo-entry中的偏移量，请跟换新的slave.h头文件并启用该行代码

            datalen = 0;
            for (int m = 0; m < maxsubindex; m++)
            {
                SDO_Upload(slave, (uint8 *)&pdodata,
                           (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->index, m + 1, 1);

                (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_index[m] = (pdodata >> 16) & 0xFFFF; // 读出映射主参数
                (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_subindex[m] = (pdodata >> 8) & 0xFF; // 读出映射子索引
                (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->pdo_data_offset[m] = datalen;            // pdodata在自己所在的pdo-entry中的偏移量
                /*请更换头文件slave.h，然后启用该行代码*/
                datalen += (pdodata & 0xFF) / 8;
            }

            (slave->pdo_channel_list[j].pdo_mapping->pdo_entry + k)->index_offset = datalen;
        }
    }
}
