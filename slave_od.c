#include "slave_od.h"


/**
 * @description: 为对象字典某一个主索引对象构建子索引链表，通过本地读取从站对象字典（例如：object.c .h）的方式（临时的方法）
 * @param {EntryListHead_t} *l --待插入的子索引链表
 * @param {SubIndex_t *} obj_subindex--本地从站对象字典中的子索引结构体数组首地址
 * @param {unsigned short} subindex_num--子索引个数，包含0号索引
 * @return {*}
 */
int AddEntryList (EntryListHead_t *l, SubIndex_t * obj_subindex,unsigned short subindex_num)
{
   nc_sdo_entry_dnode_t * p = (nc_sdo_entry_dnode_t *)malloc(sizeof(nc_sdo_entry_dnode_t)*subindex_num);//子索引结构体
    if (NULL == p)
        return 0;

    if (NULL == l)
        return 0;

    if (NULL == obj_subindex)
        return 0;
    /*将子索引结构体根据本地从站对象字典（指的是object.c .h等，是临时的处理办法）初始化，并插入子索引链表中*/
    for (unsigned short i = 0; i < subindex_num; i++)
    {
    p->bit_length=obj_subindex->ObjectBytes*8;
    p->data_type=obj_subindex->DataType;
    p->name=obj_subindex->sName;
    //p->read_access 暂时不判断权限问题
    p->Rxpdo_mapping_flag=0;
    p->Rxpdo_mapping_flag=0;//此标志位也先不处理
    p->subindex=i;
    p->pData=obj_subindex->pData;

    p->next = l->LastNode;
    p->pre = l->LastNode->pre;
    l->LastNode->pre->next = p;
    l->LastNode->pre = p;
    l->length++;
    p++;
    obj_subindex++;
    }
    return 1;
}


/**
 * @description: 将某一主索引对象根据object.c .h初始化并插入主索引链表中
 * @param {ListHead_t} *l--主索引链表
 * @param {Index_t*} obj_index--object.c中的主索引结构体指针
 * @param {nc_slave_t} *slave--对象所属的从站
 * @return {*}
 */
int AddList (ListHead_t *l, Index_t* obj_index, nc_slave_t *slave)
{
    dnode *p = NULL;

    if (NULL == l)
        return 0;

    p = (dnode *)malloc(sizeof(dnode));
    if (NULL == p)
        return 0;



    p->index = obj_index->Index;
    p->name   = obj_index->pSubIndex->sName;
    p->max_subindex =obj_index->MaxSubindex ;
    p->object_code  = obj_index->ObjectCode;
    p->slave     =  slave;
    //p->confuguration=0;
    p->entryhead = CreatEntryList();//创建一个初始的子索引链表（包含两个哨兵节点）
    if(!AddEntryList(p->entryhead,obj_index->pSubIndex,obj_index->SubindexNum))
    {
        printf("Add EntryList error!\n");
        return 0;
    }

    p->next = l->LastNode;
    p->pre = l->LastNode->pre;
    l->LastNode->pre->next = p;
    l->LastNode->pre = p;
    l->length++;
    return 1;
}





/**
 * @description: 根据object.c .h初始化主站协议栈的从站对象字典结构体主函数
 * @param {nc_slave_t *} slave--待初始化的从站
 * @param {Index_t**} index_table
 * @param {unsigned short} object_num
 * @return {*}
 */
int slave_od_init(nc_slave_t * slave,Index_t** index_table,unsigned short object_num)
{
    Index_t * obj_index;
    SubIndex_t * obj_subindex;
    slave->dictsionary = CreateList();
    ListHead_t * slave_od=slave->dictsionary;
    for(unsigned short i = 0; i < object_num; i++)
    {
        obj_index=index_table[i];
        if(!AddList(slave_od,obj_index,slave))
        {
            printf("add list error!\n");
            return 0;
        }
    }
    return 1;
    
}

/**
 * @description: 根据主索引找到从站对象字典链表中的对应元素
 * @param {ListHead_t *} dictionary
 * @param {unsigned short} index
 * @return {*}
 */
dnode * od_scanIndex(ListHead_t * dictionary,unsigned short index)
{
    dnode *obj_index = dictionary->FirstNode;
    for (unsigned long i =0; i<dictionary->length;i++)
    {
        obj_index = obj_index->next;
        if(obj_index->index == index)
        {
            return obj_index;
        }
    }
    printf("od_scanIndex:can't find index %4x in the dictionary!\n",index);
    return 0;
    
}

/**
 * @description: 根据子索引找到主索引结构体下的子索引链表中的对应子索引
 * @param {dnode *} dnode_temp
 * @param {unsigned char} subindex
 * @return {*}
 */
nc_sdo_entry_dnode_t * dnode_scanSubindex(dnode * dnode_temp, unsigned char subindex)
{
    nc_sdo_entry_dnode_t * obj_subindex;
    obj_subindex=dnode_temp->entryhead->FirstNode;
    for (unsigned short j = 0; j < dnode_temp->max_subindex; j++)
    {
        obj_subindex=obj_subindex->next;
        if (obj_subindex->subindex==subindex)
        {
            return obj_subindex;
        }
    }
    printf("od_scanIndex:can't find subindex %4x in the dnode!\n",subindex);
    return 0;
}

/**
 * @description: 根据主索引和子索引直接从对象字典中找到对应的子索引结构体
 * @param {ListHead_t *} dictionary
 * @param {unsigned short} index
 * @param {unsigned char} subindex
 * @return {*}
 */
nc_sdo_entry_dnode_t * od_scanSubindex(ListHead_t * dictionary,unsigned short index,unsigned char subindex)
{
    dnode * dnode_temp;
    dnode_temp=od_scanIndex(dictionary,index);
    return dnode_scanSubindex(dnode_temp,subindex);
}


/**
 * @description: 根据主索引和子索引读取从站对象字典内容，放入buff中
 * @param {void *} buff
 * @param {nc_slave_t*} slave
 * @param {uint16} Index
 * @param {uint8} subindex
 * @return {*}
 */
int slave_od_read(void * buff, nc_slave_t* slave,uint16 Index,uint8 subindex)
{
    
    nc_sdo_entry_dnode_t * obj_subindex;
    obj_subindex=od_scanSubindex(slave->dictsionary,Index,subindex);
    if (!obj_subindex)
    {
        return 0;
    }
    
    memcpy(buff,obj_subindex->pData,(obj_subindex->bit_length)/8);                          
    return 1;
    
}

/**
 * @description: 根据主索引和子索引将buff中的内容写从站对象字典中
 * @param {void*} buff
 * @param {nc_slave_t*} slave
 * @param {uint16} Index
 * @param {uint8} subindex
 * @return {*}
 */
int slave_od_write(const void* buff,nc_slave_t* slave,uint16 Index,uint8 subindex)
{
    
    nc_sdo_entry_dnode_t * obj_subindex;
    obj_subindex=od_scanSubindex(slave->dictsionary,Index,subindex);
    if (!obj_subindex)
    {
        return 0;
    }
    
    memcpy(obj_subindex->pData,buff,(obj_subindex->bit_length)/8);   
    
    return (1);
}


/**
 * @description: 根据从站的从站对象字典映射参数的配置，初始化slave对应的pdo_entry
 * @param {nc_slave_t*} slave
 * @param {pdo_object_mapping_t *} pdo_object_mapping_temp
 * @return {*}
 */
int pdo_entry_init(nc_slave_t* slave, pdo_object_mapping_t * pdo_object_mapping_temp)
{
    if (pdo_object_mapping_temp->pdo_entry==NULL)
    {
        printf("pdo_entry_init error: pdo_object_mapping_temp->pdo_entry is null!\n");
         return 0;
    }
   
    
    unsigned short pdo_mapping_index=pdo_object_mapping_temp->pdo_mapping_category_ID;  /*如0x1c00，而0x1c00中存放的是映射参数对象的主索引，可有多个，如0x1600 0x1601*/
    unsigned char  pdo_mapping_subindex=0;                                             /*0x1c00的子索引，用于循环扫描其整个子索引*/
    unsigned short pdo_mapping_parameters_index=0;                                    /*如0x1600,也是一个中间变量，不断循环变化*/
    unsigned char pdo_mapping_parameters_subindex=0;                                 /*0x1600的子索引*/
    unsigned char pdo_mapping_parameters_subindex_num=0;                            /*0x1600的子索引个数，不含0号索引，用于循环整个子索引列表*/
    unsigned long mapping_parameters=0;                                            /*映射参数：如从0x1600 0x01中读出60400120*/
    unsigned short mapping_object_index=0;                                        /*由映射参数解析得到的index*/
    unsigned char mapping_object_subindex=0;                                     /*由映射参数解析得到的subindex*/
    nc_sdo_entry_dnode_t * mapping_object_dnode;                                /*根据映射对象的主索引和子索引找到的映射对象结构体*/
    pdo_entries_t * pdo_entry_temp;                                            /*待初始化的pdo_entry指针*/
    pdo_entry_temp=pdo_object_mapping_temp->pdo_entry;                        /*temp指针便于后续代码编写和偏移的实现*/  
    //printf("success in EntryListHead\n");
    for (unsigned short i = 0; i < pdo_object_mapping_temp->number_of_Entries; i++)
    {
       pdo_mapping_subindex=i+1;
       slave_od_read(&pdo_mapping_parameters_index,slave,pdo_mapping_index,pdo_mapping_subindex);/*读出来如0x1600*/
       pdo_entry_temp->index=pdo_mapping_parameters_index;/*0x1600*/
       slave_od_read(&pdo_mapping_parameters_subindex_num,slave,pdo_mapping_parameters_index,0); /*读出来1600的有效子索引个数*/
       //为映射对象的主索引子索引申请空间
       pdo_entry_temp->index_offset=0;
       pdo_entry_temp->max_subindex=pdo_mapping_parameters_subindex_num;
       pdo_entry_temp->name=0;//先不指定名称

       pdo_entry_temp->pdo_index=(uint16*)malloc(sizeof(uint16)*pdo_mapping_parameters_subindex_num);
       if (NULL == pdo_entry_temp->pdo_index)
        return 0;

       pdo_entry_temp->pdo_subindex=(uint8*)malloc(sizeof(uint8)*pdo_mapping_parameters_subindex_num);
       if (NULL == pdo_entry_temp->pdo_subindex)
        return 0;

       pdo_entry_temp->pdo_data=(void**)malloc(sizeof(void *)*pdo_mapping_parameters_subindex_num);
       if (NULL == pdo_entry_temp->pdo_data)
        return 0;

       pdo_entry_temp->pdo_data_length=(uint8*)malloc(sizeof(uint8)*pdo_mapping_parameters_subindex_num);

       for (unsigned char j = 0; j < pdo_mapping_parameters_subindex_num; j++)
       {
        slave_od_read(&mapping_parameters,slave,pdo_mapping_parameters_index,j+1);/*读出映射参数*/
        mapping_object_index=(mapping_parameters >> 16) & 0xFFFF;
        mapping_object_subindex=(mapping_parameters >>  8 ) & 0xFF;

        *(pdo_entry_temp->pdo_index+j)=mapping_object_index;
        *(pdo_entry_temp->pdo_subindex+j)=mapping_object_subindex;
        mapping_object_dnode=od_scanSubindex(slave->dictsionary,mapping_object_index,mapping_object_subindex);
        *(pdo_entry_temp->pdo_data+j)=mapping_object_dnode->pData;
        *(pdo_entry_temp->pdo_data_length+j)=mapping_object_dnode->bit_length/8;
        pdo_entry_temp->index_offset+=mapping_object_dnode->bit_length/8;
       }
       pdo_entry_temp++;       
    }
    
    
    return 1;
}


/**
 * @description: 将从站的pdo_channel_data根据映射关系与对象字典相应的项做绑定：即将映射对象的指针按照映射顺序，绑定到slave->pdo_channel_list->pdo_channel_data中
 * @param {nc_slave_t} *slave
 * @return {*}
 */
int slave_bind_pdoChannelData(nc_slave_t *slave)
{
    pdo_channel_t* pdo_channel_temp=slave->pdo_channel_list;                    /*循环变量用于遍历所有的pdo通道*/
    uint8 pdo_channel_count=slave->pdo_channel_count;                           /*pdo个数，用于遍历pdo通道*/

    /*遍历所有pdo通道的大循环*/
    for (uint8 i = 0; i < pdo_channel_count; i++)
    {
        /*由映射参数计算通道长度*/
        uint16 entry_count=pdo_channel_temp->pdo_mapping->number_of_Entries;
        uint16 pdo_mappingObject_channel_length=0;

        for (uint16 j = 0; j < entry_count; j++)
        {
            pdo_mappingObject_channel_length+=((pdo_channel_temp->pdo_mapping->pdo_entry)+j)->index_offset;/*遍历某一pdo通道对应的所有映射参数主索引（如1600，1601），计算出通道长度*/
        } 

        //printf("channel length:%d\n",pdo_mappingObject_channel_length);
        if (!pdo_mappingObject_channel_length==pdo_channel_temp->channel_length)
        {
            printf(" error:the configured channel length is different from the total length calculated by the PDO mapping!\n");
            return 0;
        }
        /*********************/

        pdo_channel_temp->pdo_channel_data=(uint8*)malloc(sizeof(uint8)*pdo_mappingObject_channel_length);/*根据计算的得到的通道长度，为通道数据申请内存*/
        memset(pdo_channel_temp->pdo_channel_data,0,pdo_mappingObject_channel_length);
        uint16  bind_offset=0;/*绑定过程中的偏移量*/
        pdo_entries_t * pdo_entry_temp=pdo_channel_temp->pdo_mapping->pdo_entry;//中间变量，用于遍历pdo_entries
        nc_sdo_entry_dnode_t * mapping_object_dnode;//映射对象的结构体
        for (uint16 entry_count_temp = 0; entry_count_temp < entry_count; entry_count_temp++)
        {
            /*对于每一个entry，遍历其所有子索引（映射参数）*/
            for (uint8 subindex_temp = 0; subindex_temp < pdo_entry_temp->max_subindex; subindex_temp++)
            {
                mapping_object_dnode=od_scanSubindex(slave->dictsionary,pdo_entry_temp->pdo_index[subindex_temp],pdo_entry_temp->pdo_subindex[subindex_temp]);/*根据映射参数主索引和子索引找到对应的结构体*/
                mapping_object_dnode->pData=pdo_channel_temp->pdo_channel_data+bind_offset;/*将对象字典实际对象的指针挂到pdo_channel_data的相应位置*/
                bind_offset+=mapping_object_dnode->bit_length/8;/*偏移量+长度*/
            }

            pdo_entry_temp++;
        }
        pdo_channel_temp++;
    }
    

    return 1;
}





////////
//临时的初始化slave信息的函数，用于测试
////////







/**
 * @description: 初始化从站的各种信息（临时的函数）
 * @param {nc_slave_t} *slave
 * @param {Index_t**} index_table
 * @param {unsigned short} object_num
 * @return {*}
 */
int slave_init(nc_slave_t *slave,Index_t** index_table,unsigned short object_num)
{
    
    memset(slave,0,sizeof(nc_slave_t));
    slave->station_address=0;
    slave->ring_position=0;
    slave->service_channel_count=2;
    slave->pdo_channel_count=2;
    slave->pdo_channel_list=(pdo_channel_t *)malloc(sizeof(pdo_channel_t )*slave->pdo_channel_count);
    slave_od_init(slave,index_table,object_num);//从站的对象字典赋值
    //PrintList(slave->dictsionary);
    
    pdo_channel_t * pdo_channel_temp;
    pdo_channel_temp=slave->pdo_channel_list;
    pdo_channel_temp->category_ID=0x1002;
    pdo_channel_temp->channel_enable=0x80;
    pdo_channel_temp->channel_length=12;
    pdo_channel_temp->channel_start_address=0x0800;
    pdo_channel_temp->IRQ_enable=0;
    pdo_channel_temp->mapping_start_address=0;//这个是和报文映射访问相关的，暂时其实没用
    pdo_channel_temp->pdo_mapping_ID=0;//和报文映射访问相关，暂时没用
    pdo_channel_temp->pdo_mapping=(pdo_object_mapping_t *)malloc(sizeof(pdo_object_mapping_t ));
    pdo_object_mapping_t * pdo_mapping_temp=pdo_channel_temp->pdo_mapping;
    pdo_mapping_temp->number_of_Entries=1;
    pdo_mapping_temp->pdo_mapping_category_ID=0x1c00;//mapping category ID
    pdo_mapping_temp->pdo_entry=(pdo_entries_t *)malloc(sizeof(pdo_entries_t)*pdo_mapping_temp->number_of_Entries);
    if(!pdo_entry_init(slave,pdo_mapping_temp))
    {
        return 0;
    }
    

    pdo_channel_temp++;

    pdo_channel_temp->category_ID=0x1003;
    pdo_channel_temp->channel_enable=0x80;
    pdo_channel_temp->channel_length=12;
    pdo_channel_temp->channel_start_address=0x800+36;
    pdo_channel_temp->IRQ_enable=0x10;
    pdo_channel_temp->mapping_start_address=0;//这个是和报文映射访问相关的，暂时其实没用
    pdo_channel_temp->pdo_mapping_ID=0;//和报文映射访问相关，暂时没用
    pdo_channel_temp->pdo_mapping=(pdo_object_mapping_t *)malloc(sizeof(pdo_object_mapping_t ));
    pdo_mapping_temp=pdo_channel_temp->pdo_mapping;
    pdo_mapping_temp->number_of_Entries=1;
    pdo_mapping_temp->pdo_mapping_category_ID=0x1c01;//mapping category ID
    pdo_mapping_temp->pdo_entry=(pdo_entries_t *)malloc(sizeof(pdo_entries_t)*pdo_mapping_temp->number_of_Entries);
    if(!pdo_entry_init(slave,pdo_mapping_temp))
    {
        return 0;
    }
    

    return 1;
}


/**
 * @description: 打印pdo_entries基本信息的函数
 * @param {pdo_entries_t} *entry
 * @return {*}
 */
void print_pdo_entries(pdo_entries_t *entry) {
    printf("index: %d\n", entry->index);
    printf("max_subindex: %d\n", entry->max_subindex);
    printf("index_offset: %d\n", entry->index_offset);
    printf("index_length: %d\n", entry->index_length);
    printf("pdo_index: ");
    for (int i = 0; i < entry->max_subindex; i++) {
        printf("%4x ", entry->pdo_index[i]);
    }
    printf("\n");
    printf("pdo_subindex: ");
    for (int i = 0; i < entry->max_subindex; i++) {
        printf("%2x ", entry->pdo_subindex[i]);
    }
    printf("\n");
    printf("name: %s\n", entry->name);
    for (int i = 0; i < entry->max_subindex; i++) {
        printf("pdo_data_length: %d\n", entry->pdo_data_length[i]);
    }
    for (int i = 0; i < entry->max_subindex; i++) {
        printf("pdo_data: %d\n", *(uint32*) (entry->pdo_data[i]));
    }
    
}
