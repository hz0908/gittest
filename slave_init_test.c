/*
 * @Author: hz0908 348067333@qq.com
 * @Date: 2023-04-25 20:16:44
 * @LastEditors: hz0908 348067333@qq.com
 * @LastEditTime: 2023-04-26 14:46:23
 * @FilePath: \NC_MASTER_PDO\slave_init_test.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "slave_od.h"
#include "Object.h"
//gcc .\Object.c .\sdo.c .\slave_od.c .\slave_od_test.c -o hh
int main()
{
//printf("num:%d\n",sizeof(pObject));    
nc_slave_t * slave ;
slave=(nc_slave_t*)malloc(sizeof(nc_slave_t));
if(!slave_init(slave,pObject,ObjectNum))
{
   printf("slave init error!\n");
   return 0;
}
if (!slave->dictsionary)
{
    printf("error\n");
}
PrintList(slave->dictsionary);
print_pdo_entries(slave->pdo_channel_list->pdo_mapping->pdo_entry);
print_pdo_entries(((slave->pdo_channel_list)+1)->pdo_mapping->pdo_entry);
unsigned long * buff;
slave_od_read(buff,slave,0x1600,1);
printf("readdata:%x\n",*buff);
unsigned long  writebuff=10;
slave_od_write(&writebuff,slave,0x1600,1);
slave_od_read(buff,slave,0x1600,1);
printf("readdata:%x\n",*buff);
return 0;
}