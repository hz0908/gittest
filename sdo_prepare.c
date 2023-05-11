#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "format.h"
#include "slave.h"
#include "sdo.h"
#include "sdo_prepare.h"
// 创造主索引链表
ListHead_t *CreateList()
{
    ListHead_t *l = (ListHead_t *)malloc(sizeof(ListHead_t));
    if (NULL == l)
        printf("failed to have ListHead");

    l->FirstNode = (dnode *)malloc(sizeof(dnode)); // 设置主索引链表第一个节点
    if (NULL == l->FirstNode)
        printf("failed to have FirstNode");
    memset(l->FirstNode, 0, sizeof(dnode));

    l->LastNode = (dnode *)malloc(sizeof(dnode)); // 设置主索引链表最后一个节点
    if (NULL == l->LastNode)
        printf("failed to have LastNode");
    memset(l->LastNode, 0, sizeof(dnode));

    l->FirstNode->next = l->LastNode;
    l->LastNode->pre = l->FirstNode;
    l->length = 0;
    return l;
}

// 创造子索引链表
EntryListHead_t *CreatEntryList()
{
    EntryListHead_t *l = (EntryListHead_t *)malloc(sizeof(EntryListHead_t));
    if (NULL == l)
        printf("failed to have ListHead");

    l->FirstNode = (nc_sdo_entry_dnode_t *)malloc(sizeof(nc_sdo_entry_dnode_t)); // 设置子索引链表第一个节点
    if (NULL == l->FirstNode)
        printf("failed to have FirstNode");
    memset(l->FirstNode, 0, sizeof(nc_sdo_entry_dnode_t));

    l->LastNode = (nc_sdo_entry_dnode_t *)malloc(sizeof(nc_sdo_entry_dnode_t)); // 设置子索引链表最后一个节点
    if (NULL == l->LastNode)
        printf("failed to have LastNode");
    memset(l->LastNode, 0, sizeof(nc_sdo_entry_dnode_t));

    l->FirstNode->next = l->LastNode;
    l->LastNode->pre = l->FirstNode;
    l->length = 0;
    return l;
}

// 子索引节点初始化
void nc_sdo_entry_init(
    nc_sdo_entry_dnode_t *entry, // sdo entry
    uint8 subindex               // subindex
)
{
    entry->subindex = subindex;
    entry->data_type = 0x0000;
    entry->bit_length = 0;
    entry->read_access[NC_SDO_ENTRY_ACCESS_PREOP] = 0;
    entry->read_access[NC_SDO_ENTRY_ACCESS_SAFEOP] = 0;
    entry->read_access[NC_SDO_ENTRY_ACCESS_OP] = 0;
    entry->write_access[NC_SDO_ENTRY_ACCESS_PREOP] = 0;
    entry->write_access[NC_SDO_ENTRY_ACCESS_SAFEOP] = 0;
    entry->write_access[NC_SDO_ENTRY_ACCESS_OP] = 0;
}

// 向entry链表中添加新的子索引节点，返回1添加成功，返回0添加失败
int AppendEntryList(EntryListHead_t *l, nc_sdo_entry_dnode_t *sdo)
{
    nc_sdo_entry_dnode_t *p = sdo;

    if (NULL == l)
        return 0;

    if (NULL == p)
        return 0;

    p->next = l->LastNode;
    p->pre = l->LastNode->pre;
    l->LastNode->pre->next = p;
    l->LastNode->pre = p;
    l->length++;
    return 1;
}

/*这段代码实现了向一个双向链表中添加一个新的dnode节点，该节点保存了一个对象字典的索引号（index）、名称（name）、最大子索引号（max_subindex）、
对象代码（object_code）以及一个指向该索引号所属的从站（nc_slave_t）的指针（slave）。

具体实现过程如下：

首先，检查链表是否存在，如果链表不存在，则返回0表示添加失败。

然后，通过malloc函数为新的节点分配内存空间，如果分配失败，则返回0表示添加失败。

接着，给新的节点p赋值：将index、name、max_subindex、object_code和slave分别赋值为函数输入参数中的值。

接下来，将新的节点p添加到链表中，这里使用了双向链表。具体过程如下：

将新的节点p的next指针指向链表中的LastNode节点，也就是新节点成为最后一个节点。
将新的节点p的pre指针指向LastNode节点的前一个节点，也就是新节点的前一个节点成为当前的最后一个节点。
将新节点的前一个节点的next指针指向新节点p，也就是将新节点插入到链表的最后一个节点的前面。
将LastNode的pre指针指向新节点p，也就是将新节点插入到链表的最后一个节点的后面。
最后，链表长度加1。
最后，函数返回1表示添加成功*/
// 向主索引链表中内部添加新的主索引节点，且初始化这个主索引节点，slacve，index是外部输入的，返回1添加成功，返回0添加失败
int AppendList(ListHead_t *l, uint16 index, nc_slave_t *slave)
{
    dnode *p = NULL;

    if (NULL == l)
        return 0;

    p = (dnode *)malloc(sizeof(dnode));
    if (NULL == p)
        return 0;

    p->index = index;
    p->name = NULL;
    p->max_subindex = 0;
    p->object_code = 0;
    p->slave = slave;

    p->next = l->LastNode;
    p->pre = l->LastNode->pre;
    l->LastNode->pre->next = p;
    l->LastNode->pre = p;
    l->length++;
    return 1;
}

/*
更新节点信息
这是一个C语言函数，其作用是在给定的链表中查找与参数node的index相同的节点，并将该节点的一些信息（max_subindex，name和object_code）更新为参数node的值。

具体的实现方式是使用一个指针p遍历链表中的每个节点，当p的index值等于参数node的index时，即找到了目标节点，将该节点的相关信息更新后，函数返回1表示操作成功；如果整个链表中都没有与参数node的index相同的节点，则函数返回0表示操作失败。

需要注意的是，这个函数并不会修改链表的长度，也不会修改链表节点的指针关系。
*/
// 更新主索引某节点信息:输入主索引链表，主索引节点，返回1更新成功，返回0更新失败
int CompleteList(ListHead_t *l, dnode *node)
{
    dnode *p = l->FirstNode;
    for (int i = 0; i < l->length; i++)
    {
        p = p->next;
        if (p->index == node->index)
        {
            p->max_subindex = node->max_subindex;
            p->name = node->name;
            p->object_code = node->object_code;
            p->confuguration = node->confuguration;
            return 1;
        }
    }
    printf("Didn't find the right Sdo Info");
    return 0;
}

// 打印主索引链表
void PrintList(ListHead_t *l)
{
    dnode *p = NULL;
    int i = 0;

    if (NULL == l)
        return;

    p = l->FirstNode;
    while (i < l->length)
    {
        p = p->next;
        printf("index:%#x\n object_code:%#x max_subindex:%d name:%s\n", p->index, p->object_code, p->max_subindex, p->name);
        PrintEntryList(p->entryhead);
        ++i;
    }
}

// 打印EntryList子索引链表
void PrintEntryList(EntryListHead_t *l)
{
    nc_sdo_entry_dnode_t *p = NULL;
    int i = 0;

    // 检验输入是否为空
    if (NULL == l)
        return;

    p = l->FirstNode;
    while (i < l->length)
    {
        p = p->next;
        printf("subindex:%d data_type:%#x bit_length:%d\n", p->subindex, p->data_type, p->bit_length);
        ++i;
    }
}

// nc_sdo_request初始化
nc_sdo_request_t *InitReq(nc_sdo_request_t *req, uint16 index, uint8 subindex, uint16 mem_size, uint16 data_size, uint8 *data, uint8 number)
{
    req->index = index;
    req->subindex = subindex;
    req->mem_size = mem_size;
    req->data_size = data_size;
    req->data = data;
    req->number = number;
    return req;
}

// nc_sdo_request清空数据
void nc_sdo_request_clear_data(nc_sdo_request_t *req)
{
    if (req->data)
    {
        free(req->data);
        req->data = NULL;
    }
    req->mem_size = 0;
    req->data_size = 0;
}

// nc_sdo_request分配内存大小，输入requset和需要的存储区域大小
int nc_sdo_request_alloc(nc_sdo_request_t *req, uint16 size)
{
    if (size <= req->mem_size)
        return 0;

    nc_sdo_request_clear_data(req);

    if (!(req->data = (uint8 *)malloc(size * sizeof(uint8))))
    {
        printf("Failed to allocate %d bytes of SDO memory.\n", size);
        return -NOMEM;
    }

    req->mem_size = size;
    req->data_size = 0;
    return 0;
}

// nc_sdo_request中加入数据，输入requset，原始数据和需要的存储区域大小
int nc_sdo_request_copy_data(nc_sdo_request_t *req, const uint8 *source, uint16 size)
{
    int ret = nc_sdo_request_alloc(req, size);
    if (ret < 0)
        return ret;

    memcpy(req->data, source, size);
    req->data_size = size;
    return 0;
}

uint8 nc_Txpdo_assign(uint16 *Txpdo_index, uint8 len, nc_slave_t *slave)
{
    int i, ret;
    uint8 *data = (uint8 *)malloc(2 * (len + 1) * sizeof(uint8));
    NC_WRITE_U16(data, len);
    for (i = 0; i < len; i++)
    {
        NC_WRITE_U16(data + 2 * (i + 1), Txpdo_index[i]);
    }
    ret = SDO_Download(slave, data, 0x1C13, 0, 1, (len + 1) * 2); // 1c13与ncuc寄存器文档的59页有关
    free(data);
    return len;
}

uint8 nc_Rxpdo_assign(uint16 *Rxpdo_index, uint8 len, nc_slave_t *slave)
{
    int i, ret;
    uint8 *data = (uint8 *)malloc(2 * (len + 1) * sizeof(uint8));
    NC_WRITE_U16(data, len);
    for (i = 0; i < len; i++)
    {
        NC_WRITE_U16(data + 2 * (i + 1), Rxpdo_index[i]);
    }
    ret = SDO_Download(slave, data, 0x1C12, 0, 1, (len + 1) * 2);
    free(data);
    return len;
}
