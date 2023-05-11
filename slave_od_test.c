#include <stdio.h>
#include "slave_od.h"
#include "Object.h"
//gcc .\Object.c .\sdo.c .\slave_od.c .\slave_od_test.c -o hh
int main()
{
//printf("num:%d\n",sizeof(pObject));    
nc_slave_t * slave = (nc_slave_t *)malloc(sizeof(nc_slave_t));
if (!slave_od_init(slave,pObject,ObjectNum))
{
    return 0;
}

PrintList(slave->dictsionary);
unsigned long * buff;
slave_od_read(buff,slave,0x1600,1);
printf("readdata:%x\n",*buff);
unsigned long  writebuff=10;
slave_od_write(&writebuff,slave,0x1600,1);
slave_od_read(buff,slave,0x1600,1);
printf("readdata:%x\n",*buff);
return 0;
}