#ifndef SLAVE
#define SLAVE

#include "type.h"


typedef enum{
    NC_DIR_INVALID,
    NC_DIR_OUTPUT,
    NC_DIR_INPUT
} nc_dir_t;
/*对应的就是应用层从站信息区的固定内容部分：0x400-0x43f*/
typedef struct slave_basic_info slave_basic_info_t;
struct slave_basic_info{
    uint32 Vendor_ID;
    uint32 Product_ID;
    uint32 Revision_Num;
    uint32 Serial_Number;
    uint32 DeviceType;
    uint8  Node_ID;
    uint32 Channel_Protocol;
    uint16 Service_req_channel_start_address;
    uint16 Service_req_channel_length;
    uint8 Service_req_channel_IRQ_Enable;
    uint8 Service_req_channel_Enable;
    uint16 Service_res_channel_start_address;
    uint16 Service_res_channel_length;
    uint8 Service_res_channel_IRQ_Enable;
    uint8 Service_res_channel_Enable;
    uint8 Sync_Supported;
    uint16 Sync_Pulse_Width;
    uint32 Minimum_Cycle_Time;
    uint8 Latch_Signal_Supported;
    uint8 Slave_Watchdog_Supported;
};

/*ncuc从站芯片的一些基本信息*/
typedef struct nc_info nc_info_t;
struct nc_info{
    uint16 vendor;          
    uint16 product_code;
    uint16 version;
    uint16 revision;

    uint8 dclksupport;
    uint8 pdcsupport;
    uint8 sdcsupport;
    uint8 fifosupport;
    uint8 mapsupport;
    uint8 ramsize;
};

typedef struct pdo_entries pdo_entries_t;
struct pdo_entries{
	uint16 index;//从站对象字典中pdo映射参数的主索引如1600
    uint8  max_subindex;//最大子索引数量,不包含索引0
    uint16 index_offset;//读取的时候一个偏移量 是1600对应的所有映射对象的长度总和
    uint8  index_length;//子索引对对象的长度？单个的长度，用来做中间变量
    uint16 *pdo_index;//pdo通道对应的映射对象的主索引数组 如 [6040 6041 6002]
    uint8  *pdo_subindex;//pdo通道对应的映射对象的子索引数组头,如 [1 1 1]
    
	char * name;//主索引名称？entry->name = slave->strings[string_ID-16];从站指定的一个名称
    uint8 * pdo_data_length;//对应于每个pdo数据的字节长度
    void ** pdo_data;//pdo数据的一个快速映射索引
};

/*Address	Parameter	Data Type	Value/Description
0x00	Category ID	Unsigned16	0x1C00-0x1C1F
0x02	Category Length(Len)	Unsigned16	Number of Entries * 2, >= 2
0x04	PDO 0:	Unsigned16	[15:0] Index: 1600~17FF
Or 1A00~1BFF
0x06	PDO 1:	Unsigned16	[15:0] Index: 1600~17FF
Or 1A00~1BFF
……
	PDO Last:	Unsigned16	[15:0] Index: 1600~17FF
Or 1A00~1BFF*/
typedef struct pdo_object_mapping pdo_object_mapping_t;
struct pdo_object_mapping{
	uint16 pdo_mapping_category_ID;//Table 6- Category PDO Object Mapping List Structure  这个pdo关于对象字典的mapping对应了一个类型id
	uint16 number_of_Entries;//这个id对应的pdo entries个数
	pdo_entries_t * pdo_entry;
};

typedef struct pdo_channel pdo_channel_t;
struct pdo_channel{
    uint16 category_ID;//通道编号
    uint16 pdo_mapping_ID;//映射编号  MAPNo
	pdo_object_mapping_t * pdo_mapping;//pdo的关于对象字典的映射信息
	uint16 channel_start_address;//通道内起始地址
	uint16 channel_length;//通道长度
	uint8  IRQ_enable;//中断配置
	uint8  channel_enable;//通道开关
    uint16 mapping_start_address;//报文内起始地址
    uint8 * pdo_channel_data;//用以存放过程数据通道的
};

struct nc_slave{
    nc_master_t *master;//所属的主站
    uint16 ring_position;//顺序地址
    uint16 station_address;//站地址
    nc_info_t info;//从站fpga资源的基本信息
    slave_basic_info_t * basic_info;//从站应用基本信息
	
    // Strings
    char **strings; /**< Strings in SII categories. slave information interface*/
    uint8 string_count; /**< Number of SII strings. */
    char * Vendor_Name;
    char * Device_Name;
    char * Latch_Event_Name;

   //channel
    uint8 pdo_channel_count;//使用的pdo通道的数量
    uint8 service_channel_count;//使用的服务通道的数量,是通过从站信息
    pdo_channel_t * pdo_channel_list;//pdo通道的信息 数组

    //CON SDO
    struct ListHead * dictsionary;//从站的对象字典

};

bool station_address_config(uint16 station_address,uint16 ring_position);
bool slave_info_acquire(uint16 station_address,nc_info_t *info);


#endif // SLAVE

