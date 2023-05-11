/*
 * @Author: hz0908 348067333@qq.com
 * @Date: 2023-04-25 20:16:44
 * @LastEditors: hz0908 348067333@qq.com
 * @LastEditTime: 2023-04-26 16:39:53
 * @FilePath: \NC_MASTER_PDO\slave_init_test.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "bufferSendrecv.h"
#include "frame.h"
#include "format.h"
#include "datagram.h"
#include "register.h"
#include "process_data_channel.h"
#include "service_data_channel.h"
#include "service_req_resp_channel.h"
#include "channel_config_parameter.h"
#include "channel_view.h"
#include "slave_od.h"
#include "Object.h"
#include "reverse_array.h"
//gcc .\Object.c .\sdo.c .\slave_od.c .\slave_bind_test.c -o hj
//gcc ./Object.c ./sdo.c ./slave_od.c ./slave_bind_test.c -o hj
// gcc bufferSendrecv.c test_pro_ch.c datagram.c channel_view.c format.c frame.c process_data_channel.c register.c reverse_array.c  -o ch
// gcc bufferSendrecv.c test_pro_ch.c datagram.c channel_view.c format.c frame.c process_data_channel.c register.c reverse_array.c  -o ch
// gcc bufferSendrecv.c  datagram.c channel_view.c format.c frame.c process_data_channel.c register.c   reverse_array.c   Object.c sdo.c slave_od.c slave_bind_test.c -o ch
 static uint16 ring_position = 0x00;
bool flag=0;
// 命令行ifconfig -a 查看
// ifconfig enp2s0 promisc 将接收端口开启为混杂模式，这样才能收到不符合mac地址的报文
// gcc bufferSendrecv.c test_pro_ch.c datagram.c channel_view.c format.c frame.c process_data_channel.c register.c reverse_array.c  -o ch
// 测试变量区
char send_card_name[] = "enp1s0";
char rev_card_name[] = "enp2s0";

int main()
{
      // 套接字创建
  send_socketCreate(send_card_name);
  rev_socketCreate(rev_card_name);
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
//PrintList(slave->dictsionary);
slave_bind_pdoChannelData(slave);
//print_pdo_entries(slave->pdo_channel_list->pdo_mapping->pdo_entry);
//print_pdo_entries(((slave->pdo_channel_list)+1)->pdo_mapping->pdo_entry);

uint32* control_word;
uint32* status_word;
uint32* target_velocity;
uint32* current_velocity;
uint32* target_position;
uint32* current_position;




 // // 通道配置
 pdo_channel_t * pdo_channel =slave->pdo_channel_list;
//  for (size_t i = 0; i < 12; i++)
// {
//     printf("pdo_channel data%d:%x\n",i,pdo_channel->pdo_channel_data[i]);
// }
proc_data_channel_config_apwr(ring_position, pdo_channel->channel_start_address ,pdo_channel->channel_length, pdo_channel->IRQ_enable, pdo_channel->category_ID-0x1000);
proc_data_channel_on_apwr(ring_position, pdo_channel->category_ID-0x1000);
channel_view(ring_position,pdo_channel->category_ID-0x1000 );
control_word=(uint32*)pdo_channel->pdo_channel_data;
target_position=(uint32*)(pdo_channel->pdo_channel_data+4);
target_velocity=(uint32*)(pdo_channel->pdo_channel_data+8);

 pdo_channel++;

//   for (size_t i = 0; i < 12; i++)
// {
//     printf("pdo_channel data%d:%x\n",i,pdo_channel->pdo_channel_data[i]);
// }
//,
proc_data_channel_config_apwr(ring_position, pdo_channel->channel_start_address ,pdo_channel->channel_length, pdo_channel->IRQ_enable, pdo_channel->category_ID-0x1000);
proc_data_channel_on_apwr(ring_position, pdo_channel->category_ID-0x1000);
channel_view(ring_position,pdo_channel->category_ID-0x1000 );
status_word=(uint32*)pdo_channel->pdo_channel_data;
current_position=(uint32*)(pdo_channel->pdo_channel_data+4);
current_velocity=(uint32*)(pdo_channel->pdo_channel_data+8);

pdo_channel =slave->pdo_channel_list;
uint8 irqflag=0;
//printf("addr status word:%x\n",status_word);
//printf("addr control word:%x\n",control_word);
while (1)
{
    for (uint8 i = 0; i < slave->pdo_channel_count; i++)
    {
        irqflag=nc_slave_aprd_uint8(ring_position,0x0127+(pdo_channel->category_ID-0x1002)*16,&flag);
        if (!flag)
        {
            printf("irqflag read error!\n");
            return 0;
        }
        // if (irqflag)
        // {
        //      printf("irqflag:%x\n",irqflag);
        // }
        
       
        if (irqflag&0x01)
        {
            if (pdo_channel->IRQ_enable>>4&0x01)//主站读从站写
            {
                printf("channel number%x:\n",pdo_channel->category_ID);
                 proc_data_channel_achrd(ring_position,pdo_channel->pdo_channel_data,pdo_channel->channel_length,pdo_channel->category_ID-0x1000,0);
                
            }
            else
            {
                printf("channel number%x:\n",pdo_channel->category_ID);
                proc_data_channel_achwr(ring_position,pdo_channel->pdo_channel_data,pdo_channel->channel_length,pdo_channel->category_ID-0x1000,0);
            }
            
        }
        
        pdo_channel++;
         
    }
    pdo_channel =slave->pdo_channel_list;
    (*control_word)++;
    (*target_position)++;
    (*target_velocity)++;
    printf("status word:%d  current position:%d  current velocity:%d\n",*status_word,*current_position,*current_velocity);
    printf("controlword:%d  target position :%d  target  velocity:%d\n",*control_word,*target_position,*target_velocity);
    usleep(1000000);
}


// unsigned long * buff;
// slave_od_read(buff,slave,0x6040,1);
// printf("readdata:%x\n",*buff);
// unsigned long  writebuff=10;
// slave_od_write(&writebuff,slave,0x6040,1);
// slave_od_read(buff,slave,0x6040,1);
// printf("readdata:%x\n",*buff);
// slave->pdo_channel_list->pdo_channel_data[1]=1;
// slave_od_read(buff,slave,0x6040,1);
// printf("readdata:%x\n",*buff);

 send_rev_socketClose();
return 0;
}