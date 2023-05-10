#ifndef SDO_PREPARE
#define SDO_PREPARE
#include "type.h"

enum
{
    NC_SDO_ENTRY_ACCESS_PREOP,  // PREOP状态的access权力
    NC_SDO_ENTRY_ACCESS_SAFEOP, // SAFEOP状态下的access权力
    NC_SDO_ENTRY_ACCESS_OP,     // OP状态下的access权力
    NC_SDO_ENTRY_ACCESS_COUNT   // 状态数量
};

/*对象字典中的子索引结构体
typedef struct
{
    unsigned short      DataType;
    unsigned char       AccessFlag;
    unsigned char       ObjectBytes;
    void                *pData;
    char                *sName;

}   SubIndex_t;*/

// 制作entry节点
struct nc_sdo_entry_dnode
{
    uint8 subindex;                                // 子索引
    uint16 data_type;                              // 数据类型
    uint16 bit_length;                             // 位的长度
    uint8 read_access[NC_SDO_ENTRY_ACCESS_COUNT];  // 读取权力
    uint8 write_access[NC_SDO_ENTRY_ACCESS_COUNT]; // 写入权力
    /*Object Access	Unsigned8
        Bit 0:Pre-OP状态下的读访问
        Bit 1:Safe-OP状态下的读访问
        Bit 2:OP状态下的读访问
        Bit 3:Pre-OP状态下的写访问
        Bit 4:Safe-OP状态下的写访问
        Bit 5:OP状态下的写访问
        Bit 6:在RxPDO中对象可映射
        Bit 7:在TxPDO中对象可映射*/
    char *name; // 描述
    uint8 Rxpdo_mapping_flag;
    uint8 Txpdo_mapping_flag;
    struct nc_sdo_entry_dnode *pre;  // 前一个entry节点
    struct nc_sdo_entry_dnode *next; // 后一个entry节点
};
typedef struct nc_sdo_entry_dnode nc_sdo_entry_dnode_t; // 取别名

// 制作entry链表
struct EntryListHead
{
    nc_sdo_entry_dnode_t *FirstNode; // 第一个entry节点
    nc_sdo_entry_dnode_t *LastNode;  // 最后一个entry节点
    int length;                      // entry节点数量
};
typedef struct EntryListHead EntryListHead_t; // 取别名

// 制作主索引节点
struct doubleLink
{
    uint16 index;               // 主索引
    uint8 object_code;          // 目标代码 7：Variable 变量 8：Array 数组 9：Record  记录
    char *name;                 // 对象字典中，每个子索引对象对应的一个描述。如： "communication error"
    nc_slave_t *slave;          // 所属从站
    uint8 max_subindex;         // 最大子索引数，包含了0号子索引——表征有效子索引数量的子索引对象
    uint8 confuguration;        // 主站可配置，主站对对象字典的读写权力
    EntryListHead_t *entryhead; // entry链表
    struct doubleLink *pre;     // 前一个主索引对象
    struct doubleLink *next;    // 后一个主索引对象
};
typedef struct doubleLink dnode; // 取别名

// 制作主索引链表
struct ListHead
{
    dnode *FirstNode; // 第一个主索引节点
    dnode *LastNode;  // 最后一个主索引节点
    int length;       // 主索引节点数量
};
typedef struct ListHead ListHead_t; // 取别名

// request准备参数
struct nc_sdo_request
{
    uint16 index;            // sdo index
    uint8 subindex;          // sdo subindex
    uint8 *data;             // CoN SDO Data的指针
    uint16 mem_size;         // Size of CoN SDO Data memory
    uint16 data_size;        // Size of Data
    uint8 number;            // 子索引数量
    uint32 issue_timeout;    // Maximum time in ms, the processing of the request may take.
    uint32 response_timeout; // Maximum time in ms, the transfer is retried, if the slave does not respond.
    int _errno;              // Error number.
    uint32 abort_code;       // SDO request abort code. Zero on success.
};
typedef struct nc_sdo_request nc_sdo_request_t; // 取别名

ListHead_t *CreateList();                                            // 创造主索引链表
EntryListHead_t *CreatEntryList();                                   // 创造子索引链表
void nc_sdo_entry_init(nc_sdo_entry_dnode_t *entry, uint8 subindex); // 子索引节点初始化
int AppendEntryList(EntryListHead_t *l, nc_sdo_entry_dnode_t *sdo);  // 向entry链表中添加新的子索引节点，返回1添加成功，返回0添加失败
int AppendList(ListHead_t *l, uint16 index, nc_slave_t *slave);      // 向主索引链表中内部添加新的主索引节点，且初始化这个主索引节点，slacve，index是外部输入的，返回1添加成功，返回0添加失败
int CompleteList(ListHead_t *l, dnode *node);                        // 更新主索引某节点信息:输入主索引链表，主索引节点，返回1更新成功，返回0更新失败
void PrintList(ListHead_t *l);                                       // 打印主索引链表
void PrintEntryList(EntryListHead_t *l);                             // 打印EntryList子索引链表

nc_sdo_request_t *InitReq(nc_sdo_request_t *req, uint16 index, uint8 subindex,
                          uint16 mem_size, uint16 data_size, uint8 *data, uint8 number); // nc_sdo_request初始化
void nc_sdo_request_clear_data(nc_sdo_request_t *req);                                   // nc_sdo_request清空数据
int nc_sdo_request_alloc(nc_sdo_request_t *, uint16);                                    // nc_sdo_request分配内存大小，输入requset和需要的存储区域大小
int nc_sdo_request_copy_data(nc_sdo_request_t *, const uint8 *, uint16);                 // nc_sdo_request中加入数据，输入requset，原始数据和需要的存储区域大小
uint8 nc_Txpdo_assign(uint16 *Txpdo_index, uint8 len, nc_slave_t *slave);                // 为一个或多个PDO对象分配一个或多个传输对象，并返回PDO数量
uint8 nc_Rxpdo_assign(uint16 *Rxpdo_index, uint8 len, nc_slave_t *slave);                // 为一个或多个PDO对象分配一个或多个传输对象，并返回PDO数量

#endif // SDO_PREPARE
