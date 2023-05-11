/*
 * @Author: hz0908 348067333@qq.com
 * @Date: 2023-04-25 11:25:48
 * @LastEditors: hz0908 348067333@qq.com
 * @LastEditTime: 2023-04-26 16:00:17
 * @FilePath: \NC_MASTER_PDO\slave_od.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef SLAVE_OD
#define SLAVE_OD

#include "Object.h"
#include "slave.h"
#include "sdo.h"
#include "OD.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int slave_od_init(nc_slave_t * slave,Index_t** index_table,unsigned short object_num);
int slave_od_read(void* buff, nc_slave_t* slave,uint16 Index,uint8 subindex);
int slave_od_write( const void* buff,nc_slave_t* slave,uint16 Index,uint8 subindex);
int slave_init(nc_slave_t *slave,Index_t** index_table,unsigned short object_num);
void print_pdo_entries(pdo_entries_t *entry);
int slave_bind_pdoChannelData(nc_slave_t *slave);



#endif