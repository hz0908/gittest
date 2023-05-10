#ifndef SDO
#define SDO
#include "type.h"
enum {
    NC_SDO_ENTRY_ACCESS_PREOP, /**< Access rights in PREOP. */
    NC_SDO_ENTRY_ACCESS_SAFEOP, /**< Access rights in SAFEOP. */
    NC_SDO_ENTRY_ACCESS_OP, /**< Access rights in OP. */
    NC_SDO_ENTRY_ACCESS_COUNT /**< Number of states. */
};
/*描述的就是对象字典中的子索引结构体
typedef struct
{
    unsigned short      DataType;

    unsigned char       AccessFlag;
    unsigned char       ObjectBytes;
    void                *pData;
    char                *sName;

}   SubIndex_t;*/
struct nc_sdo_entry_dnode{
//    struct list_head list; /**< List item. */
    uint8 subindex; /**< Subindex. */ 
    uint16 data_type; /**< Data type. */
    uint16 bit_length; /**< Data size in bit. */
    void   *pData;//data
    uint8 read_access[NC_SDO_ENTRY_ACCESS_COUNT]; /**< Read access. */
    uint8 write_access[NC_SDO_ENTRY_ACCESS_COUNT]; /**< Write access. */
    /*Object Access	Unsigned8	Bit 0:Pre-OP状态下的读访问
        Bit 1:Safe-OP状态下的读访问
        Bit 2:OP状态下的读访问
        Bit 3: Pre-OP状态下的写访问
        Bit 4:Safe-OP状态下的写访问
        Bit 5:OP状态下的写访问
        Bit 6:在RxPDO中对象可映射
        Bit 7:在TxPDO中对象可映射*/
    char *name; /**< Description. */
    uint8 Rxpdo_mapping_flag;
    uint8 Txpdo_mapping_flag;
    struct nc_sdo_entry_dnode *pre;
    struct nc_sdo_entry_dnode *next;
} ;
typedef struct nc_sdo_entry_dnode nc_sdo_entry_dnode_t;

/*子索引链表*/
struct EntryListHead
{
    nc_sdo_entry_dnode_t *FirstNode;//第一个子索引对象入口
    nc_sdo_entry_dnode_t *LastNode;//最后一个子索引对象入口
    int length;//总共有多少个子索引
};
typedef struct EntryListHead EntryListHead_t;


/*doublelink dnode 是主索引信息*/
struct doubleLink
{
    uint16 index;//子索引属于的主索引
    uint8 object_code;//对象代码 7：Variable 变量 8：Array   数组 9：Record  记录
    char *name;//对象字典中，每个子索引对象对应的一个描述，如： "communication error"
    nc_slave_t *slave;//所属的从站
    uint8 max_subindex;//最大子索引数，包含了0号子索引——表征有效子索引个数的子索引对象
    uint8 confuguration;//主站可配置到底是什么？就是主站对该对象字典的读写权限
    EntryListHead_t * entryhead;//关于子索引链表的入口
    struct doubleLink *pre;//前一个主索引对象
    struct doubleLink *next;//后一个主索引对象
};
typedef struct doubleLink dnode;

/*listhead是一本对象字典的实现（通过链表的方式实现的）*/
struct ListHead
{
    dnode	*FirstNode;//第一个对象（主索引）
    dnode   *LastNode;//最后一个对象（主索引）
    int length;//总共有多少个主索引
};
typedef struct ListHead ListHead_t;
int AppendEntryList(EntryListHead_t * l,nc_sdo_entry_dnode_t * sdo);
int CompleteList(ListHead_t *l,dnode * node);
ListHead_t *CreateList() ;
EntryListHead_t *CreatEntryList();
int AppendList(ListHead_t *l,uint16 index, nc_slave_t *slave);
//int AppendList (ListHead *l, uint16 index, ec_slave_t *slave);
void PrintList(ListHead_t *l)  ;
void nc_sdo_entry_init(nc_sdo_entry_dnode_t *entry,uint8 subindex);

#endif // SDO

