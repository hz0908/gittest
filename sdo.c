#define NC_CON_DOWN_REQ_PREPARE_SIZE 10
#define NC_CON_DOWN_REQ_SEG_PREPARE_SIZE 4
#define NC_SERVICE_CHANNEL_COE 0x00000001
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// #include <QMessageBox>
#include "slave.h"
#include "service_req_resp_channel.h"
#include "type.h"
#include "format.h"
#include "sdo_prepare.h"
#include "sdo.h"
#include "datagram.h"
// #include "channel_view.h"

/*这段代码是一个从CANopen设备中获取对象字典信息的函数。它接收一个指向设备的指针 slave 作为输入，
并在执行期间使用 nc_fsm_coe_t 结构体保存状态信息。
函数使用 nc_fsm_coe_dict_start 函数来初始化 nc_fsm_coe_t 结构体，并生成用于请求设备对象字典信息的数据报文。
然后，它使用 service_channel_request_by_ring_position 函数将数据报文发送到设备。
在等待设备响应期间，函数不断检查 status_service_channel_respond_by_ring_position 函数的返回值，
以判断设备是否已经发送了响应报文。一旦报文接收完成，
它将使用 service_channel_respond_by_ring_position 函数读取报文数据并将其存储在 datagram_res 缓冲区中。
最后，函数将使用 nc_fsm_coe_dict_response 函数分析报文数据，并根据其内容更新状态信息。
当函数收到所有请求的响应后，它将使用 desc_func 函数来解析对象字典，并将结果保存在设备的 dictionary 字段中。
最后，函数释放 nc_fsm_coe_t 结构体并返回结果。*/
int Sdo_Info_Get(nc_slave_t *slave)
{
    //	printf("select a funciont\n 1:   GET OD LIST\n  2:   GET OD DESC\n 3:   GET ENTRY DESC\n");
    int finished = 1;
    int ret;
    channel_status_t res_channel_flag = EMPTY;
    uint8 *datagram_req = (uint8 *)malloc(slave->basic_info->Service_req_channel_length * sizeof(uint8));
    uint8 *datagram_res = (uint8 *)malloc(slave->basic_info->Service_res_channel_length * sizeof(uint8));
    memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
    memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
    nc_fsm_coe_t *fsm = (nc_fsm_coe_t *)malloc(sizeof(nc_fsm_coe_t));
    memset(fsm, 0, sizeof(nc_fsm_coe_t));
    fsm->slave = slave;
    ret = nc_fsm_coe_dict_start(fsm, datagram_req); // 0x0005 Command Unsigned8 0x01：Get OD List请求
    if (ret == 0)
    {
        // 发送request
        if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
        {
            printf("slave %d :having problem in request_channel transmitting.\n", slave->ring_position);
            free(fsm);
            return 0;
        }
        // 已经发送完成
        slave->dictsionary = CreateList(); // 创造主索引链表
        printf("Wait Get-OD-List response:\n");
        while (finished) // 循环接收所有的OD list(index)
        {
            res_channel_flag = EMPTY;
            while (res_channel_flag != FULL)
            {
                res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position);
            }

            if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
            {
                printf("slave %d :having problem in respond_channel transmitting.", slave->ring_position);
                free(fsm);
                return 0;
            }
            finished = nc_fsm_coe_dict_response(fsm, datagram_res);
        }
        printf("Get-OD-List successful!\n\n");
    }
    else
    {
        free(fsm);
        return 0;
    }
    desc_func(slave, fsm); // 填充entry和name等信息
    //    PrintList(slave->dictionary);
    free(fsm);
    return 1;
}

/*这段代码实现了nc_fsm_coe_dict_start()函数，
它的功能是启动COE（CANopen over EtherCAT）的字典，
初始化COE通道传输字典的相关参数。
具体来说，它首先通过参数fsm获取一个指向nc_slave_t类型的结构体指针slave，
判断该从设备是否支持COE通道协议，
如果不支持则输出错误信息并返回1；
如果支持则调用nc_fsm_coe_prepare_dict()函数初始化COE通道传输字典Get OD List Request，
并返回0表示成功启动字典。*/
int nc_fsm_coe_dict_start(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    if (!(slave->basic_info->Channel_Protocol & NC_SERVICE_CHANNEL_COE))
    {
        printf("Slave %d does not support CoN!\n", slave->ring_position);
        return 1;
    }
    nc_fsm_coe_prepare_dict(datagram);
    return 0;
}

/*这段代码是在准备一个 COE（CANopen over EtherCAT）协议的数据报文，
用于获取从设备对象字典（Object Dictionary）的信息。
具体来说，nc_fsm_coe_prepare_dict() 函数设置了数据报文中的一些字段，
包括通道数据长度、服务类型、命令字和一些其他信息。最后，函数返回 0，表示数据报文准备成功。
这里涉及到一些宏定义，如 NC_WRITE_U16 和 NC_WRITE_U8，它们是用来进行数据的类型转换和字节序转换的，
这些转换是由于在不同的平台上，数据的存储方式可能会不同，例如大端序和小端序。这些宏定义的具体实现需要根据平台来确定。*/
int nc_fsm_coe_prepare_dict(
    uint8 *datagram /**< Datagram to use. */)
{
    NC_WRITE_U16(datagram, 0x06);    // channel data length
    NC_WRITE_U8(datagram + 2, 0x01); // CoN service 协议类型
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x01);
    NC_WRITE_U8(datagram + 5, 0x01);  // 0x01:Get OD List Request
    NC_WRITE_U16(datagram + 6, 0x01); // deliver all SDOs!

    //   fsm->state = ec_fsm_coe_dict_request;
    return 0;
}

/*这段代码是一个名为service_channel_request_by_ring_position的函数，用于在通信总线上向特定从设备发送请求消息，
并等待从设备的响应消息。具体地，该函数会构造一个包含请求数据的帧，发送到从设备的环上，并等待从设备返回的数据帧。
如果接收到的数据帧中包含无效的 IRQ（Interrupt Request）字段，或者 IRQ 中的响应码不是 "1"（表示正常响应），
则函数返回 false 表示通信失败；否则函数返回 true 表示通信成功。*/
bool service_channel_request_by_ring_position(uint16 ring_position, uint16 size, uint8 *data, uint16 len_ch)
{
    int16 result;
    uint16 irq;
    nc_frame_t frame;
    initframe(&frame);
    if (size > len_ch)
        return false;
    // bool proc_data_channel_achrd(uint16 ring_position,uint8 *data,uint16 len,uint8 ch_No)
    /**正常运作的话 result的值应该为0**/
    result = chAccessdgrmAdd(&frame, ACHWR, ring_position, 0x00, size, SVR_REQ_CH, data, 0);
    if (result < 0)
        return false;

    FrameTransmit(&frame);
    if (FrameReceive(&frame) < 0)
    {
        return false;
    }
    /**检测irq**/
    /**frame.dgrambuff + result 为本datagram起始地址，加7之后跳过datagram头部，加(size + 2)跳过数据区，指向IRQ**/
    irq = nctohs(*(uint16 *)(frame.dgrambuff + result + 7 + size));
    // printf("%#x,%#x\n",irq,(irq & 0x03));
    if ((irq & 0x03) != 0x01)
        return false;
    return true;
}

/*这段代码是CANopen通信协议栈的一部分，用于处理从从设备接收到请求设备对象字典（OD）时收到的响应。
当收到响应时，函数nc_fsm_coe_dict_response被调用，它以nc_fsm_coe_t结构体表示通信状态机和uint8数组datagram表示接收到的数据作为输入参数。

该函数首先使用函数nc_slave_res_channel_fetch检索响应数据，并检查使用的邮箱协议是否为CoE（CANopen over EtherCAT）。
如果不是CoE，则函数返回错误代码。它还检查是否接收到紧急消息，如果是，则返回错误代码。

如果接收到的响应包含SDO（服务数据对象）信息，则函数检查是否接收到错误响应，如果是，则打印错误消息并返回错误代码。
否则，它检查是否为Get OD List请求的响应，如果是，则继续提取OD中对象的索引列表。

该函数检查接收到的数据是否具有预期的大小和格式，然后提取索引并将其附加到从设备的OD列表中。
最后，它检查是否还有更多的分片需要接收，并返回适当的代码。
AppendList(slave->dictsionary,sdo_index,slave);获得的是index列表*/
int nc_fsm_coe_dict_response(
    nc_fsm_coe_t *fsm,
    uint8 *datagram) // return 2出现错误，return 1检查到后面还有分段，return 0无碍
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot; // mbox_port是Type,0x10:CoN  data是CoN Header之后的所有
    uint16 rec_size;        // rec_size是通道长度-2，前两位保存的长度
    uint32 sdo_count, i;
    uint16 sdo_index, fragments_left;
    uint16 index_list_offset;

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size); // 给data,mbox_port,rec_size填充

    if (mbox_prot != 0x01) // CoE 是否是SDO information
    {
        printf("Slave%d Received mailbox protocol 0x%02X as response.\n", fsm->slave->ring_position,
               mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size)) // 检查是否是emergency
        return 2;

    if (rec_size < 9) // 是否有index list
    {
        printf("Slave %d: Received corrupted SDO dictionary response (size %d).\n", slave->ring_position, rec_size);
        return 2;
    }

    // 检查是否是SDO info Error Request
    if (NC_READ_U8(data) == 0x1 &&             // SDO information
        (NC_READ_U8(data + 1) & 0x7F) == 0x07) // 0x07:SDO Info Error Request
    {
        printf("Slave %d: SDO information error response!\n", fsm->slave->ring_position);
        if (rec_size < 8) // 4+4=8，errocode是32位的
        {
            printf("Slave %d: Incomplete SDO information error response:\n", fsm->slave->ring_position);
        }
        else
        {
            nc_canopen_abort_msg(slave, NC_READ_U32(data + 2)); // data+2就是终止代码
        }
        return 2;
    }

    // 检查是否是Get OD List response
    if (NC_READ_U8(data) != 0x1 ||             // SDO information 命令
        (NC_READ_U8(data + 1) & 0x7F) != 0x02) // 简单来看就是02&02=02 服务子命令中0x02:Get OD List Response
    {
        printf("Slave %d: didn't get the response", fsm->slave->ring_position);
        return 2;
    }

    //   first_segment = list_empty(&slave->sdo_dictionary) ? true : false;
    //   index_list_offset = first_segment ? 8 : 6;
    index_list_offset = 6; // CoN Header和CoN SDO Data中index list之外的字节数

    if (rec_size < index_list_offset || rec_size % 2)
    {
        printf("Slave %d :Invalid data size %d!\n", fsm->slave->ring_position, rec_size);
        return 2;
    }

    sdo_count = (rec_size - 2 - index_list_offset) / 2; // index list中index的数量

    // 将Get OD List Request中的index写入到slave->dictsionary中
    for (i = 0; i < sdo_count; i++)
    {
        sdo_index = NC_READ_U16(data + index_list_offset + i * 2); // 拿出index
        if (!sdo_index)
        {
            printf("Slave %d: SDO dictionary contains index 0x0000.\n", fsm->slave->ring_position);
            continue;
        }
        AppendList(slave->dictsionary, sdo_index, slave); // 向dictionary字典中添加一个主索引节点 在循环添加前已经完成了字典链表的新建
        printf("No.%d index:%#04x\n", fsm->segment_size + i + 1, sdo_index);
    }

    fragments_left = NC_READ_U8(data + 4); // 0x0008	Fragments Left	Unsigned8	后面分段的数量
    if (fragments_left)
    {
        printf("Slave %d: SDO list fragments left: %u Waiting...\n", slave->ring_position, fragments_left);
        fsm->segment_size += sdo_count;
        return 1; // 检查到后面还有分段，则return 1
    }

    else
    {
        fsm->segment_size = 0;
        return 0;
    }
}

/*这是一个检查CANopen over EtherCAT (CoE)通信中是否收到了紧急消息的函数，它接受三个参数：nc_fsm_coe_t结构体，
表示通信状态机；一个uint8型数据数组，表示接收到的数据；以及一个uint16型变量，表示数据数组的长度。

函数首先检查数据数组的长度是否大于等于2，以及第一个字节是否为0x04，如果不符合条件，函数将返回0，表示没有收到紧急消息。

如果满足条件，则继续检查数据数组的长度是否小于12，如果小于12，则打印一条错误消息，并输出数据数组中的每个字节的值，最后返回1，表示收到了不完整的紧急消息。
3)Emergency Request
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	0x0C:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x04：Emergency
0x0005	Command	Unsigned8	0x00
CoN SDO Data
0x0006	Error Code	Unsigned16	错误代码
0x0008	Error Register	B Unsigned8	错误寄存器
0x0009	Data	BYTE[5]	Manufacturer specific Error Field*/

int nc_fsm_coe_check_emergency(
    nc_fsm_coe_t *fsm,
    const uint8 *data,
    uint16 size)
{
    if (size < 2 || (NC_READ_U8(data)) != 0x04) // 对应0004h-emergency命令
        return 0;

    if (size < 12)
    {
        printf("Slave %d: Received incomplete CoE Emergency", fsm->slave->ring_position);
        for (int i = 0; i < size; i++)
        {
            printf("%d\t", NC_READ_U8(data + i));
        }
        return 1;
    } // emergency 长度有一个最小值，但似乎也不是12啊

    printf("Slave %d CoE Emergency Request received:\nError code 0x%04X, Error register 0x%02X, data:\n", fsm->slave->ring_position, NC_READ_U16(data + 2), NC_READ_U8(data + 4));
    //   ec_print_data(data + 5, 5);
    printf("%d\t%d\t%d\t%d\t%d\t", NC_READ_U8(data + 5), NC_READ_U8(data + 6), NC_READ_U8(data + 7), NC_READ_U8(data + 8), NC_READ_U8(data + 9));
    return 1;
}

// 终止代码的集合，前面是终止代码，后面是描述
const nc_code_msg_t sdo_abort_messages[] = {
    {0x05030000, "Toggle bit not changed"},
    {0x05040000, "SDO protocol timeout"},
    {0x05040001, "Client/Server command specifier not valid or unknown"},
    {0x05040005, "Out of memory"},
    {0x06010000, "Unsupported access to an object"},
    {0x06010001, "Attempt to read a write-only object"},
    {0x06010002, "Attempt to write a read-only object"},
    {0x06020000, "This object does not exist in the object directory"},
    {0x06040041, "The object cannot be mapped into the PDO"},
    {0x06040042, "The number and length of the objects to be mapped would"
                 " exceed the PDO length"},
    {0x06040043, "General parameter incompatibility reason"},
    {0x06040047, "Gerneral internal incompatibility in device"},
    {0x06060000, "Access failure due to a hardware error"},
    {0x06070010, "Data type does not match, length of service parameter does"
                 " not match"},
    {0x06070012, "Data type does not match, length of service parameter too"
                 " high"},
    {0x06070013, "Data type does not match, length of service parameter too"
                 " low"},
    {0x06090011, "Subindex does not exist"},
    {0x06090030, "Value range of parameter exceeded"},
    {0x06090031, "Value of parameter written too high"},
    {0x06090032, "Value of parameter written too low"},
    {0x06090036, "Maximum value is less than minimum value"},
    {0x08000000, "General error"},
    {0x08000020, "Data cannot be transferred or stored to the application"},
    {0x08000021, "Data cannot be transferred or stored to the application"
                 " because of local control"},
    {0x08000022, "Data cannot be transferred or stored to the application"
                 " because of the present device state"},
    {0x08000023, "Object dictionary dynamic gabort_msg->message;eneration fails or no object"
                 " dictionary is present"}};

// 输入从站信息与某个终止代码，轮询查终止代码集合，看看从站的终止代码是什么原因
void nc_canopen_abort_msg(
    const nc_slave_t *slave, /**< Slave. */
    uint32 abort_code        /**< Abort code to search for. */
)
{
    const nc_code_msg_t *abort_msg;

    for (abort_msg = sdo_abort_messages; abort_msg->code; abort_msg++)
    {
        if (abort_msg->code == abort_code)
        {
            //            printf("Slave %d: SDO abort message 0x%08X: \"%s\".\n",slave->ring_position,
            //                   abort_msg->code, abort_msg->message);

            // QString类型的str定义，并且初始化。Qt框架中的类型QString
            // QString::number(slave->ring_position)：将整数 slave->ring_position 转换成字符串，并将其与前面的字符串连接
            // QString str = "Slave" + QString::number(slave->ring_position) + ":SDO abort message 0x" + QString::number(abort_msg->code) + ":" + abort_msg->message;
            // 使用 QMessageBox::warning() 方法弹出一个警告对话框，该对话框包含了一个标题（"Error"）和一个消息文本（str 变量中的字符串内容）。第一个参数为 NULL，表示对话框不依赖于任何父窗口。第二个参数为标题，表示对话框的标题。
            // QMessageBox::warning(NULL, "Error", str);
            printf("Slave %d SDO aobrt message 0x %#x :%s", slave->ring_position, abort_msg->code, abort_msg->message);
            return;
        }
    }

    printf("Slave %d: Unknown SDO abort code 0x%08X.\n", slave->ring_position, abort_code);
}

// GetObject Description Requset  GetEntry Description Requset
void desc_func(nc_slave_t *slave, nc_fsm_coe_t *fsm)
{
    uint8 *datagram_req = (uint8 *)malloc(slave->basic_info->Service_req_channel_length * sizeof(uint8));
    uint8 *datagram_res = (uint8 *)malloc(slave->basic_info->Service_res_channel_length * sizeof(uint8));
    memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
    memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
    ListHead_t *l = slave->dictsionary;
    dnode *p = l->FirstNode; // 首个主索引节点（不填数据）
    int ret;
    int finished = 1;
    channel_status_t res_channel_flag = EMPTY;
    for (int i = 0; i < slave->dictsionary->length; i++)
    {
        finished = 1;
        p = p->next;                                           // 首个主索引节点之后的节点（填写数据的首个主索引节点）
        p->entryhead = CreatEntryList();                       // 填写数据的首个主索引节点存储的子索引链表
        fsm->sdo = p;                                          // fsm->sdo设置为当前主索引节点
        fsm->index = p->index;                                 // fsm->index设置为当前主索引节点的index
        ret = nc_fsm_coe_dict_prepare_desc(fsm, datagram_req); // 将通道写报文的datagram填充为GetObject Description Request（准备报文阶段）
        if (ret == 0)
        {
            // 发送request
            if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
            {
                printf("slave %d :having problem in request_channel transmitting.\n", slave->ring_position);
            }
            // 完成请求
        }
        printf("Wait index No.%d %#x Get-Object-Description response:\n", i + 1, fsm->index);
        while (finished)
        {
            res_channel_flag = EMPTY;
            while (res_channel_flag != FULL)
            {
                res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position); // 等待服务应答通道，检查通道1的状态，是否写入新数据
            }

            if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res)) // 读一下服务应答通道中的数据，放到datagram_res中
            {
                printf("slave %d :having problem in respond_channel transmitting.", slave->ring_position);
            }
            finished = nc_fsm_coe_dict_desc_response(fsm, datagram_res); // 更新fsm中的slave中的节点信息
            // 这里这个while循环应该是用来实现分段传输的，但是分段传输的代码似乎还没实现

            //						finished = ec_fsm_coe_dict_entry_response(fsm,pdg1);
        }
        for (fsm->subindex = 0; fsm->subindex < fsm->sdo->max_subindex; fsm->subindex++)
        {
            memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
            memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
            ret = nc_fsm_coe_dict_prepare_entry(fsm, datagram_req); // 将通道写报文的datagram的data区中填充为Get Entry Description Request
            if (ret == 0)
            {
                // 发送request
                if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
                {
                    printf("slave %d :having problem in request_channel transmitting.", slave->ring_position);
                }
                // 完成请求
            }
            printf("Wait index No.%d %#x subindex %d Get-Entry-Description response:\n", i + 1, fsm->index,fsm->subindex);
            finished = 1;
            while (finished)
            {
                res_channel_flag = EMPTY;
                while (res_channel_flag != FULL)
                {
                    res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position); // 等待服务应答通道，检查通道1的状态，是否写入新数据
                }

                if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
                {
                    printf("slave %d:having problem in respond_channel transmitting.", slave->ring_position);
                }
                finished = nc_fsm_coe_dict_entry_response(fsm, datagram_res); // 更新fsm中的slave中的entry节点信息
            }
        }
    }
    printf("finished Get-Object-Description and Get-Entry-Description successful!\n");
}

/*5)GetObject Description Request
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	0x06:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoNHeader
0x0004	Service	Unsigned8	0x01：SDO Information
0x0005	Command	Unsigned8	0x03:Get Object Description请求
CoN SDO Data
0x0006	Index	Unsigned16	请求对象描述的索引*/
int nc_fsm_coe_dict_prepare_desc(
    nc_fsm_coe_t *fsm, /*< Finite state machine. */
    uint8 *datagram    /** 4< Datagram to use. */
)
{
    NC_WRITE_U16(datagram, 0x06);    // channel data length
    NC_WRITE_U8(datagram + 2, 0x01); // CoN service
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x01);
    NC_WRITE_U8(datagram + 5, 0x03);
    NC_WRITE_U16(datagram + 6, fsm->index); // deliver all SDOs!
    return 0;
}

/*6)Get Object Description Response
Response (First)
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	n>0x0C:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x01：SDO Information
0x0005	Command	Unsigned8	0x04:Get Object Description响应
CoN SDO Data
0x0006 	Index	Unsigned16	对象描述的索引
0x0008	Fragments Left	Unsigned8	后面分段的数量
0x0009	Confuguration Supported	Unsigned8	主站可配置
0x000A	Max Subindex	Unsigned8	对象的最大子索引
0x000B	Object Code	Unsigned8	对象代码
7：Variable 变量
8：Array   数组
9：Record  记录
0x000C	Data Type	Unsigned16	数据类型列表的引用
0x000E	Name	char[n-12]	对象名称
这个函数看起来是获取对象名称，配置信息，最大索引号，对象码的*/
int nc_fsm_coe_dict_desc_response(
    nc_fsm_coe_t *fsm,
    uint8 *datagram)
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot;     // mbox_port是Type,0x10:CoN  data是CoN Header之后的所有
    uint16 rec_size, name_size; // rec_size是通道长度-2，前两位保存的长度
    // dnode *sdo = (dnode *)malloc(sizeof(dnode));
    //  sdo->index = 0x7000; // cia301协议文件中:7000h – 77FFh Standardized profile area 3rd logical device

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size); // 给data,mbox_port,rec_size填充

    if (mbox_prot != 0x01) // CoE 是否是SDO information
    {
        printf("Slave%d Received mailbox protocol 0x%02X as response.\n", fsm->slave->ring_position,
               mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size)) // 检查是否是emergency
    {
        // check for CoE response again
        // ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        return 2;
    }

    if (NC_READ_U8(data) == 0x1 &&             // SDO information
        (NC_READ_U8(data + 1) & 0x7F) == 0x07) // error response
    {
        printf("Slave %d:SDO information error response while fetching SDO 0x%04X!\n", slave->ring_position, fsm->sdo->index);
        nc_canopen_abort_msg(slave, NC_READ_U32(data + 2));
        return 2;
    }

    if (NC_READ_U8(data) != 0x1 ||            // SDO information
        (NC_READ_U8(data + 1) & 0x7F) != 0x04 //|| // Object desc. response  0x04:Get Object Description响应
        // NC_READ_U16(data + 2) != 0x7000
    )
    { // SDO index
        printf("Slave %d: Invalid object description response while fetching SDO 0x%04X!\n", slave->ring_position, fsm->sdo->index);
        return 2;
    }

    if (fsm->offset) // 不是first
    {
        if (rec_size < 9)
        {
            printf("Slave %d:Received corrupted SDO description response:(size %d).\n", slave->ring_position, rec_size);
            return 2;
        }
        name_size = rec_size - 8;
        fsm->sdo->name = (char *)realloc(fsm->sdo->name, (fsm->segment_size + name_size) * sizeof(char)); // name补全
        memcpy(fsm->sdo->name + fsm->segment_size, data + 6, name_size);
    }
    else // 是first
    {
        if (rec_size < 13)
        {
            printf("Slave %d:Received corrupted SDO description response:(size %d).\n", slave->ring_position, rec_size);
            return 2;
        }
        fsm->sdo->confuguration = NC_READ_U8(data + 5);
        fsm->sdo->max_subindex = NC_READ_U8(data + 6);
        fsm->sdo->object_code = NC_READ_U8(data + 7);
        name_size = rec_size - 12;                                   // 看一看Get Object Description Response中是否有对象名称Name
        fsm->sdo->name = (char *)malloc((name_size) * sizeof(char)); // 赋给sdo的name
        if (name_size)
        {
            memcpy(fsm->sdo->name, data + 10, name_size);
        }
        // int flag = CompleteList(slave->dictsionary, sdo); // 更新当前节点信息 这段不需要
    }

    // 查看后面是否跟着分段
    if (NC_READ_U8(data + 4)) // 后面还有分段
    {
        printf("Fragment follows (not implemented)!  Waiting...\n");
        fsm->offset++;                  // 借此表示GetObject Description Request,++表示不是first
        fsm->segment_size += name_size; // segment_size上加name_size
        return 1;
    }
    else // 后面无分段
    {
        printf("Index = %#x: confuguration = %d, max_subindex = %d, object_code = %#x, name = %s\n\n",
               fsm->sdo->index, fsm->sdo->confuguration, fsm->sdo->max_subindex, fsm->sdo->object_code, fsm->sdo->name);
        fsm->offset = 0;
        fsm->segment_size = 0; // 借此表示GetObject Description Request,0表示是first
        return 0;
    }
}

/*7)Get Entry Description Request
相对位置	数据字段	数据类型	值/描述
        Channel Header
0x0000	Length	WORD	0x08:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x01：SDO Information
0x0005	Command	Unsigned8	0x05:Get Entry Description请求
CoN SDO Data
0x0008	Index	Unsigned16	请求对象描述的索引
0x000A	Subindex	Unsigned8	被请求对象的子索引
0x000B	Reseved	Unsigned8
似乎文档写错了*/
int nc_fsm_coe_dict_prepare_entry(
    nc_fsm_coe_t *fsm, /**< Finite state machine */
    uint8 *datagram    /**< Datagram to use. */
)
{
    NC_WRITE_U16(datagram, 0x08);    // channel data length
    NC_WRITE_U8(datagram + 2, 0x01); // CoN service
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x01);
    NC_WRITE_U8(datagram + 5, 0x05);
    NC_WRITE_U16(datagram + 6, fsm->index);   // deliver all SDOs!
    NC_WRITE_U8(datagram + 8, fsm->subindex); // 子索引
    NC_WRITE_U8(datagram + 9, 0x00);
    return 0;
}

/*8)Get Entry Description Response
Response (First)
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	n>0x0D:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x01：SDO Information
0x0005	Reserved	Unsigned8	0x06:Get Entry Description响应
CoN SDO Data
0x0006	Index	Unsigned16	对象描述的索引
0x0008	Subindex	Unsigned8	请求对象描述的子索引
0x0009	Fragments Left	Unsigned8	后面分段的数量
0x000A	Data Type	Unsigned16	对象数据类型的索引
0x000C	Bit Length	Unsigned16	对象的位长度
0x000E	Object Access	Unsigned8
Bit 0:Pre-OP状态下的读访问
Bit 1:Safe-OP状态下的读访问
Bit 2:OP状态下的读访问
Bit 3: Pre-OP状态下的写访问
Bit 4:Safe-OP状态下的写访问
Bit 5:OP状态下的写访问
Bit 6:在RxPDO中对象可映射
Bit 7:在TxPDO中对象可映射
0x000F	Reserved	Unsigned8	保留
0x0010	Name	BYTE[n-14]	*/
int nc_fsm_coe_dict_entry_response(
    nc_fsm_coe_t *fsm,
    uint8 *datagram)
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot;     // mbox_port是Type,0x10:CoN  data是CoN Header之后的所有
    uint16 rec_size, name_size; // rec_size是通道长度-2，前两位保存的长度

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size); // 给data,mbox_port,rec_size填充

    if (mbox_prot != 0x01) // CoE 是否是SDO information
    {
        printf("Slave %d: Received mailbox protocol: 0x%02X as response.\n", slave->ring_position, mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size)) // 检查是否是emergency
    {
        // check for CoE response again
        return 2;
    }

    if (NC_READ_U8(data) == 0x1 &&             // SDO information
        (NC_READ_U8(data + 1) & 0x7F) == 0x07) // error response emergency子命令 07
    {                                          // error response
        printf("Slave %d: SDO information error response while fetching SDO entry 0x%04X:%02X!\n", slave->ring_position, fsm->index, fsm->subindex);
        nc_canopen_abort_msg(slave, NC_READ_U32(data + 2));
        return 2;

        /* There may be gaps in the subindices, so try to continue with next
         * subindex. */
    }

    if (NC_READ_U8(data) != 0x1 ||            // SDO information
        (NC_READ_U8(data + 1) & 0x7F) != 0x06 //|| // Entry desc. response
        // NC_READ_U16(data + 2) != 0x7000 ||       // SDO index
        // NC_READ_U8(data + 4) != 01
        ) // SDO subindex
    {
        /*
                    if (fsm->slave->master->debug_level) {
                        EC_SLAVE_DBG(slave, 1, "Invalid entry description response"
                                " while fetching SDO entry 0x%04X:%02X!\n",
                                sdo->index, fsm->subindex);
                        ec_print_data(data, rec_size);
                    }
        */
        // check for CoE response again
        // ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        printf("Invalid entry description response while fetching SDO entry 0x%04X:%02X!\n", 0x7000, 0x01);
        return 2;
    }

    if (fsm->offset) // 不是first
    {
        if (rec_size < 9)
        {
            printf("Slave %d : Received corrupted SDO entry :description response (size %d).\n", slave->ring_position, rec_size);
            return 2;
        }

        name_size = rec_size - 8;
        fsm->sdo->entryhead->LastNode->pre->name = (char *)realloc(fsm->sdo->entryhead->LastNode->pre->name, (fsm->segment_size + name_size) * sizeof(char)); // name补全
        memcpy(fsm->sdo->entryhead->LastNode->pre->name + fsm->segment_size, data + 6, name_size);
    }
    else // 是first
    {
        if (rec_size < 15)
        {
            printf("Slave %d : Received corrupted SDO entry :description response (size %d).\n", slave->ring_position, rec_size);
            return 2;
        }
        uint16 word;
        nc_sdo_entry_dnode_t *entry;
        if (!(entry = (nc_sdo_entry_dnode_t *)malloc(sizeof(nc_sdo_entry_dnode_t)))) // 创建子索引节点
        {
            printf("Slave %d: Failed to allocate entry!\n", slave->ring_position);
            return 2;
        }
        nc_sdo_entry_init(entry, 0x01);
        entry->data_type = NC_READ_U16(data + 6);
        entry->bit_length = NC_READ_U16(data + 8);
        // read access rights
        word = NC_READ_U8(data + 10); // 0x000E	Object Access	Unsigned8
        entry->read_access[NC_SDO_ENTRY_ACCESS_PREOP] = word & 0x0001;
        entry->read_access[NC_SDO_ENTRY_ACCESS_SAFEOP] = (word >> 1) & 0x0001;
        entry->read_access[NC_SDO_ENTRY_ACCESS_OP] = (word >> 2) & 0x0001;
        entry->write_access[NC_SDO_ENTRY_ACCESS_PREOP] = (word >> 3) & 0x0001;
        entry->write_access[NC_SDO_ENTRY_ACCESS_SAFEOP] = (word >> 4) & 0x0001;
        entry->write_access[NC_SDO_ENTRY_ACCESS_OP] = (word >> 5) & 0x0001;
        entry->Rxpdo_mapping_flag = (word >> 6) & 0x0001;
        entry->Txpdo_mapping_flag = (word >> 7) & 0x0001;
        name_size = rec_size - 14;
        entry->name = (char *)malloc((name_size) * sizeof(char));
        if (name_size)
        {
            memcpy(entry->name, data + 12, name_size);
            
        }
        AppendEntryList(fsm->sdo->entryhead, entry);
    }

    // 查看后面是否跟着分段
    if (NC_READ_U8(data + 5)) // 后面还有分段
    {
        printf("Fragment follows (not implemented)!  Waiting...\n");
        fsm->offset++;                  // 借此表示GetObject Description Request,++表示不是first
        fsm->segment_size += name_size; // segment_size上加name_size
        return 1;
    }
    else // 后面无分段
    {
        fsm->offset = 0;
        fsm->segment_size = 0; // 借此表示GetObject Description Request,0表示是first
        printf("subindex name = %s\n\n", fsm->sdo->entryhead->LastNode->pre->name);
        return 0;
    }
}

// SDO_Download服务
int SDO_Download(nc_slave_t *slave, uint8 *data, uint16 index, uint8 subindex, uint8 number, uint8 length)
{
    int finished = 0;
    channel_status_t res_channel_flag = EMPTY;
    int ret;
    uint8 *datagram_req = (uint8 *)malloc(slave->basic_info->Service_req_channel_length * sizeof(uint8));
    uint8 *datagram_res = (uint8 *)malloc(slave->basic_info->Service_res_channel_length * sizeof(uint8));
    memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
    memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
    nc_fsm_coe_t *fsm = (nc_fsm_coe_t *)malloc(sizeof(nc_fsm_coe_t));
    fsm->slave = slave;
    nc_sdo_request_t *req = (nc_sdo_request_t *)malloc(sizeof(nc_sdo_request_t));
    fsm->request = InitReq(req, index, subindex, 0, length, data, number);
    ret = nc_fsm_coe_down_start(fsm, datagram_req);
    if (ret == 0)
    {
        if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
        {
            printf("slave %d :having problem in init request_channel transmitting.\n", slave->ring_position);
            free(fsm);
            free(req);
            return 0;
        }

        printf("Wait download init response:\n");
        while (res_channel_flag != FULL)
        {
            res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position);
        }

        if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
        {
            printf("slave %d :having problem in seg respond_channel transmitting.\n", slave->ring_position);
            free(fsm);
            free(req);
            return 0;
        }
        finished = nc_fsm_coe_down_response(fsm, datagram_res);

        while (finished == 0) // 后面还有seg
        {
            memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
            memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
            res_channel_flag = EMPTY;
            nc_fsm_coe_down_prepare_segment_request(fsm, datagram_req);
            if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
            {
                printf("slave %d :having problem in request_channel transmitting.\n", slave->ring_position);
                free(fsm);
                free(req);
                return 0;
            }
            printf("Wait download seg response:\n");

            while (res_channel_flag != FULL)
            {
                res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position);
            }

            if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
            {
                printf("slave %d :having problem in respond_channel transmitting.\n", slave->ring_position);
                free(fsm);
                free(req);
                return 0;
            }

            finished = nc_fsm_coe_down_seg_response(fsm, datagram_res);
        }
        if (finished == 2)
        {
            printf("having some errors in the downloading function\n");
            free(fsm);
            free(req);
            return 0;
        }
        else
        {
            printf("\n");
            printf("finished the Download successful!\n");
            free(fsm);
            free(req);
            return 1;
        }
    }
    else
    {
        free(fsm);
        free(req);
        return 0;
    }
}

/*这个函数中调用的是request init 的datagram准备函数，在调用之前检查了协议对不对，通道长度够不够*/
int nc_fsm_coe_down_start(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    nc_sdo_request_t *request = fsm->request;

    printf("Slave %d:Download SDO index:0x%x subindex:%x number:%d.\n",
           slave->ring_position, request->index, request->subindex, request->number);

    if (!(slave->basic_info->Channel_Protocol & NC_SERVICE_CHANNEL_COE))
    {
        printf("Slave %d does not support CoN!\n", slave->ring_position);
        request->_errno = PROTONOSUPPORT;
        return 1;
    }

    if (slave->basic_info->Service_req_channel_length <
        NC_CON_DOWN_REQ_PREPARE_SIZE + 2)
    {
        printf("Slave %d: Mailbox too small!\n", slave->ring_position);
        request->_errno = OVERFLOW;
        return 1;
    }

    nc_fsm_coe_prepare_down_start(fsm, datagram);
    return 0;
}

// 相当于在拼Init Request报文，从fsm中拿需要的信息，拼到报文里面
int nc_fsm_coe_prepare_down_start(
    nc_fsm_coe_t *fsm,
    uint8 *datagram)
{
    nc_slave_t *slave = fsm->slave;
    nc_sdo_request_t *request = fsm->request;

    // 最大长度是服务请求通道最大长度 - 非数据部分12字节
    uint8 max_data_size = slave->basic_info->Service_req_channel_length - 12;
    fsm->complete_size = request->data_size; // request中的数据,即为12之后的data
    if (request->data_size > max_data_size)
    {
        // 下载的数据大小大于最大单次传输大小，需要使用段下载
        request->data_size = max_data_size; // 通道能发的最长data_size
    }
    NC_WRITE_U16(datagram, request->data_size + NC_CON_DOWN_REQ_PREPARE_SIZE); // 0x000C	Data	BYTE[N-10]数据长度+12(数据之前的一些说明占据的字节)-2(长度说明占2个字节)
    NC_WRITE_U8(datagram + 2, 0x01);
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x02); // SDO download
    NC_WRITE_U8(datagram + 5, 0x00); // Init Request
    NC_WRITE_U16(datagram + 6, request->index);
    NC_WRITE_U8(datagram + 8, request->subindex);
    NC_WRITE_U8(datagram + 9, request->number);
    NC_WRITE_U16(datagram + 10, request->data_size);
    memcpy(datagram + NC_CON_DOWN_REQ_PREPARE_SIZE + 2, request->data, request->data_size);
    fsm->offset = request->data_size;                         // fsm->offset意思是数据还需要发送的是data偏离点
    fsm->remaining = fsm->complete_size - request->data_size; // fsm->reamining意思是data还未发送的部分
    return 0;
}

/*4)Init Response
相对地址	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	0x0A:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x02:SDO Download
0x0005	Command	Unsigned8	0x01: Init Response
CoN SDO Data
0x0006	Index	Unsigned16	对象的索引
0x0008	Subindex	Unsigned8	对象的起始子索引
0x0009	Number	Unsigned8	下载的子索引数量
0x000A	Received Size	Unsigned16	已经接收的数据*/
// 接收datagram，并且进行获取数据，排错，检查emergency，是否abort
int nc_fsm_coe_down_response(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot;
    uint16 rec_size;
    nc_sdo_request_t *request = fsm->request;

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size); // 回收的data从CoN Header开始

    if (mbox_prot != 0x01)
    { // CoN
        request->_errno = IO;
        printf("Slave %d: Received channel protocol 0x%02X as response.\n", slave->ring_position, mbox_prot);
        return 2;
    } // 协议不对，errno改为IO，3

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size))
    {
        // check for CoE response again
        //        ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        return 2;
    }
    if (rec_size < 10)
    {
        request->_errno = IO;
        printf("Slave %d: Received data are too small (%d bytes):\n", slave->ring_position, rec_size);
        //        ec_print_data(data, rec_size);
        return 2;
    } // 通道长度不对，errno也改成IO，3

    if (NC_READ_U8(data) == 0x2 && // SDO Download
        NC_READ_U8(data + 1) == 0x4)
    { // abort SDO transfer request  0x0004	Service	Unsigned8	0x04：Abort SDO Transfer Request
        uint8 wrong_subindex;
        request->_errno = IO;
        wrong_subindex = NC_READ_U8(data + 4);
        printf("Slave %d: SDO download 0x%04X:0x%02X (%d bytes) aborted.\n", slave->ring_position, request->index, wrong_subindex, request->data_size);
        if (rec_size < 10)
        {
            printf("Slave %d: Incomplete abort command:\n", slave->ring_position);
        }
        else
        {
            fsm->request->abort_code = NC_READ_U32(data + 6); //  0x000A	Abort Code	Unsigned32	终止代码*/
            nc_canopen_abort_msg(slave, fsm->request->abort_code);
        }
        return 2;
    }
    /*7)Abort Transfer
    相对位置	数据字段	数据类型	值/描述
    Channel Header
    0x0000	Length	Unsigned16	0x0C:服务通道数据长度
    0x0002	Type	Unsigned8	0x01:CoN
    0x0003	Reserved	Unsigned8	0x00
    CoN Header
    0x0004	Service	Unsigned8	0x04：Abort SDO Transfer Request
    0x0005	Reserved	Unsigned8	0x00
    CoN SDO Data
    0x0006	Index	Unsigned16	出错的对象索引
    0x0008	Subindex	Unsigned8	出错的对象子索引
    0x0009	Reserved	Unsigned8	保留
    0x000A	Abort Code	Unsigned32	终止代码*/

    if (NC_READ_U8(data) != 0x2 ||                 // SDO Download
        NC_READ_U8(data + 1) != 0x1 ||             // Init response
        NC_READ_U16(data + 2) != request->index || // index
        NC_READ_U8(data + 4) != request->subindex ||
        NC_READ_U8(data + 5) != request->number || // 0x0009	Number	Unsigned8	下载的子索引数量
        NC_READ_U16(data + 6) != request->data_size)
    { // subindex
        printf("Slave %d: Invalid SDO download response! Please Retrying...\n", slave->ring_position);
        // check for CoE response again
        //        ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        return 2;
    }

    if (fsm->remaining)
    { // more segments to download
        printf("download init successful!\n");
        return 0;
    }
    else
    {
        printf("download init successful!\n");
        return 1;
    }
}

/*6)Seg Response
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	0x06:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x02：SDO Download
0x0005	Command	Unsigned8	0x03: Seg Response
CoN SDO Data
0x0006	Received Size	Unsigned16	已接受的数据*/
int nc_fsm_coe_down_seg_response(
    nc_fsm_coe_t *fsm, /**< Finite state machine */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot;
    uint16 rec_size;
    nc_sdo_request_t *request = fsm->request;

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size);

    if (mbox_prot != 0x01)
    { // CoE
        request->_errno = IO;
        printf("Slave %d: Received mailbox protocol 0x%02X as response.\n", slave->ring_position, mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size))
    {
        return 2;
    }

    if (rec_size < 6)
    {
        request->_errno = IO;
        printf("Slave %d: Received data are too small (%d bytes):\n", slave->ring_position,
               rec_size);
        return 2;
    }

    if (NC_READ_U8(data) == 0x2 && // SDO Download
        NC_READ_U8(data + 1) == 0x4)
    { // abort SDO transfer request
        uint8 wrong_subindex;
        request->_errno = IO;
        wrong_subindex = NC_READ_U8(data + 4);
        printf("Slave %d: SDO download 0x%04X:0x%02X (%d bytes) aborted.\n", slave->ring_position,
               request->index, wrong_subindex, request->data_size);
        if (rec_size < 10)
        {
            printf("Slave %d: Incomplete abort command:\n", slave->ring_position);
        }
        else
        {
            fsm->request->abort_code = NC_READ_U32(data + 6);
            nc_canopen_abort_msg(slave, fsm->request->abort_code);
        }
        return 2;
    }

    if (NC_READ_U8(data) != 0x2 ||
        (NC_READ_U8(data + 1) != 0x03))
    { // segment response
        return 2;
    }

    fsm->offset += fsm->segment_size;
    fsm->remaining -= fsm->segment_size;

    if (fsm->remaining)
    { // more segments to download
        printf("download seg successful!\n");
        return 0;
        //        ec_fsm_coe_down_prepare_segment_request(fsm, datagram);
    }
    else
    {
        printf("download seg successful!\n");
        return 1;
    }
}

/*5)Seg Request
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	N>=0x04:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x02：SDO Download
0x0005	Command	Unsigned8	0x02: Seg Request
CoN SDO Data
0x0006	Data	Data[N-4]	对象的数据部分*/
void nc_fsm_coe_down_prepare_segment_request(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    nc_sdo_request_t *request = fsm->request; // request里面存着需要下一步需要发送的请求
    uint8 max_data_size = slave->basic_info->Service_req_channel_length - 6;
    uint8 last_segment; // 这是最后一个部分的标志位

    if (fsm->remaining > max_data_size)
    {
        fsm->segment_size = max_data_size;
        last_segment = 0; // 这不是最后一个报文
    }
    else
    {
        fsm->segment_size = fsm->remaining;
        last_segment = 1;
    }
    NC_WRITE_U16(datagram, fsm->segment_size + 4); // len代表的是0002开始的区域
    NC_WRITE_U8(datagram + 2, 0x01);
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x02); // SDO Download
    NC_WRITE_U8(datagram + 5, 0x02); // Seg Request
    memcpy(datagram + NC_CON_DOWN_REQ_SEG_PREPARE_SIZE + 2,
           request->data + fsm->offset, fsm->segment_size);
}

// SDO upload读取服务
int SDO_Upload(
    nc_slave_t *slave,
    uint8 *data,
    uint16 index,
    uint8 subindex,
    uint8 number)
{
    int finished, i = 0;
    channel_status_t res_channel_flag = EMPTY;
    int ret;
    uint8 *datagram_req = (uint8 *)malloc(slave->basic_info->Service_req_channel_length * sizeof(uint8));
    uint8 *datagram_res = (uint8 *)malloc(slave->basic_info->Service_res_channel_length * sizeof(uint8));
    memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
    memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
    nc_fsm_coe_t *fsm = (nc_fsm_coe_t *)malloc(sizeof(nc_fsm_coe_t));
    fsm->slave = slave;
    nc_sdo_request_t *req = (nc_sdo_request_t *)malloc(sizeof(nc_sdo_request_t));
    fsm->request = InitReq(req, index, subindex, 0, 0, 0, number); // req初始化，赋值index,subindex,mem_size,data_size,data
    ret = nc_fsm_coe_up_start(fsm, datagram_req);
    if (ret == 0)
    {
        if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
        {
            printf("slave %d :having problem in request_channel transmitting.", slave->ring_position);
            free(fsm);
            free(req);
            return 0;
        }
        printf("Wait upload init response:\n");
        while (res_channel_flag != FULL)
        {
            res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position);
        }

        if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
        {
            printf("slave %d :having problem in init respond_channel transmitting.", slave->ring_position);
            free(fsm);
            free(req);
            return 0;
        }
        finished = nc_fsm_coe_up_response(fsm, datagram_res);
        while (finished == 0) // 后面还有seg
        {
            memset(datagram_req, 0, slave->basic_info->Service_req_channel_length * sizeof(uint8));
            memset(datagram_res, 0, slave->basic_info->Service_res_channel_length * sizeof(uint8));
            res_channel_flag = EMPTY;
            nc_fsm_coe_up_prepare_segment_request(datagram_req);
            if (!(service_channel_request_by_ring_position(slave->ring_position, slave->basic_info->Service_req_channel_length, datagram_req, slave->basic_info->Service_req_channel_length)))
            {
                printf("slave %d :having problem in request_channel transmitting.\n", slave->ring_position);
                free(fsm);
                free(req);
                return 0;
            }
            printf("Wait upload seg response:\n");

            while (res_channel_flag != FULL)
            {
                res_channel_flag = status_service_channel_respond_by_ring_position(slave->ring_position);
            }

            if (!service_channel_respond_by_ring_position(slave->ring_position, slave->basic_info->Service_res_channel_length, datagram_res))
            {
                printf("slave %d :having problem in respond_channel transmitting.", slave->ring_position);
                free(fsm);
                free(req);
                return 0;
            }
            finished = nc_fsm_coe_up_seg_response(fsm, datagram_res);
        }
        if (finished == 2)
        {
            printf("having some errors in the uploading function\n");
            free(fsm);
            free(req);
            return 0;
        }
        else
        {
            printf("\n");
            printf("Slave %d:Upload SDO index:0x%x subindex:%x number:%d\n",
                   slave->ring_position, fsm->request->index, fsm->request->subindex, fsm->request->number);

            printf("Data:0x");
            for (i = 0; i < fsm->request->data_size; i++)
                printf("%x", fsm->request->data[i]);
            printf("\n");

            memcpy(data, fsm->request->data, fsm->request->data_size);
            printf("finished the Upload successful!\n");
            free(fsm);
            free(req);
            return 1;
        }
    }
    else
    {
        free(fsm);
        free(req);
        return 0;
    }
}

/*upload Init Request*/
int nc_fsm_coe_up_start(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    nc_sdo_request_t *request = fsm->request;

    //   printf("Slave %d: Uploading SDO 0x%04X:%02X.\n",
    // slave->ring_position,request->index, request->subindex);

    if (!(slave->basic_info->Channel_Protocol & NC_SERVICE_CHANNEL_COE))
    {
        printf("Slave %d does not support CoN!\n", slave->ring_position);
        request->_errno = PROTONOSUPPORT;
        return 1;
    }

    nc_fsm_coe_prepare_up(fsm, datagram);
    return 0;
}

/*upload Init Request*/
int nc_fsm_coe_prepare_up(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_sdo_request_t *request = fsm->request;
    NC_WRITE_U16(datagram, 0x0A);
    NC_WRITE_U8(datagram + 2, 0x01); // CON
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x03); // SDO Upload
    NC_WRITE_U8(datagram + 5, 0x00); // Init Request
    NC_WRITE_U16(datagram + 6, request->index);
    NC_WRITE_U8(datagram + 8, request->subindex);
    NC_WRITE_U8(datagram + 9, request->number);
    NC_WRITE_U16(datagram + 10, 0x00);
    return 0;
}

// upload response
/*4)Init Response
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	N>=0x0A:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x03：SDO Upload
0x0005	Command	Unsigned8	0x01: Init Response
CoN SDO Data
0x0006	Index	Unsigned16	对象的索引
0x0008	Subindex	Unsigned8	对象的起始子索引
0x0009	Number	Unsigned8	下载的子索引数量
0x000A	Complete Size	Unsigned16	对象的完整数据大小
0x000C	Data	Data[N-10]	对象的数据，如果Complete Size >N-10，则随后是Upload Sdo Segment
整个函数看起来像是为request结构体里填充data*/
int nc_fsm_coe_up_response(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    uint16 rec_index;
    uint8 *data, mbox_prot, rec_subindex;
    uint16 rec_size, data_size; // rec_size 2Bytes之后的长度
    nc_sdo_request_t *request = fsm->request;
    int ret;

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size); // CoN Header之后
    /*
        if (master->debug_level) {
            EC_SLAVE_DBG(slave, 1, "Upload response:\n");
            ec_print_data(data, rec_size);
        }
    */

    if (mbox_prot != 0x01)
    { // CoN
        request->_errno = IO;
        printf("Slave %d :Received mailbox protocol 0x%02X as response.\n", slave->ring_position, mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size))
    {
        // check for CoE response again
        //       ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        return 2;
    }

    if (rec_size < 6)
    {
        request->_errno = IO;
        printf("Slave %d: Received currupted SDO upload response(%d bytes)!\n", slave->ring_position, rec_size);
        return 2;
    }

    if (NC_READ_U8(data) == 0x3 && // SDO Download
        NC_READ_U8(data + 1) == 0x4)
    { // abort SDO transfer request
        uint8 wrong_subindex;
        request->_errno = IO;
        wrong_subindex = NC_READ_U8(data + 4);
        printf("Slave %d: SDO upload 0x%04X:0x%02X (%d bytes) aborted.\n", slave->ring_position,
               request->index, wrong_subindex, request->data_size);
        if (rec_size < 10)
        {
            printf("Slave %d: Incomplete abort command:\n", slave->ring_position);
        }
        else
        {
            fsm->request->abort_code = NC_READ_U32(data + 6);
            nc_canopen_abort_msg(slave, fsm->request->abort_code);
        }
        return 2;
    }

    if (NC_READ_U8(data) != 0x3 || // SDO Upload
        NC_READ_U8(data + 1) != 0x1)
    { // Init response
        printf("Slave %d: Received unknown response while uploading SDO 0x%04X:%02X.\n",
               slave->ring_position, request->index, request->subindex);
        request->_errno = IO;
        return 2;
    }

    rec_index = NC_READ_U16(data + 2);
    rec_subindex = NC_READ_U8(data + 4);

    if (rec_index != request->index || rec_subindex != request->subindex)
    {
        printf("Slave %d: Received upload response for wrong SDO  (0x%04X:%02X, requested: 0x%04X:%02X).\n",
               slave->ring_position, rec_index, rec_subindex, request->index, request->subindex);

        // check for CoE response again
        //       ec_slave_mbox_prepare_check(slave, datagram); // can not fail.
        return 2;
    }

    fsm->complete_size = NC_READ_U16(data + 6);
    request->number = NC_READ_U8(data + 5);
    request->data_size = fsm->complete_size;
    // printf("%d \n", request->data_size);
    data_size = rec_size - 10;          // data_size是一次最多能装的data数据长度 12之后
    if (data_size < fsm->complete_size) // 通过判断通道能够存储数据的大小来修改remaining
    {
        ret = nc_sdo_request_alloc(request, fsm->complete_size); // 如果complete_size比req的mem_size大的话，重新以大的size为req->data申请内存，更新mem—size
        if (ret)
        {
            request->_errno = -ret;
            return 2;
        }

        fsm->remaining = fsm->complete_size - data_size;
        ret = nc_sdo_request_copy_data(request, data + 8, data_size); // 数据填入fsm->request->data
        if (ret)
        {
            request->_errno = -ret;
            return 2;
        }
        printf("upload init successful!\n");
        printf("Slave %d: SDO data incomplete (%d / %d). Segmenting...\n", slave->ring_position, data_size, fsm->complete_size);
        //            ec_fsm_coe_up_prepare_segment_request(fsm, datagram);
        return 0;
    }
    else
    {
        fsm->remaining = 0;
        ret = nc_sdo_request_copy_data(request, data + 8, fsm->complete_size);
        if (ret)
        {
            request->_errno = -ret;
            return 2;
        }
    }
    printf("upload init successful!\n");
    return 1;
}

void nc_fsm_coe_up_prepare_segment_request(uint8 *datagram)
{
    NC_WRITE_U16(datagram, 0x04);
    NC_WRITE_U8(datagram + 2, 0x01); // CON
    NC_WRITE_U8(datagram + 3, 0x00);
    NC_WRITE_U8(datagram + 4, 0x03); // SDO Upload
    NC_WRITE_U8(datagram + 5, 0x02); // Seg Request
}

/*Seg Response
相对位置	数据字段	数据类型	值/描述
Channel Header
0x0000	Length	Unsigned16	N>=0x04:服务通道数据长度
0x0002	Type	Unsigned8	0x01:CoN
0x0003	Reserved	Unsigned8	0x00
CoN Header
0x0004	Service	Unsigned8	0x03：SDO Upload
0x0005	Command	Unsigned8	0x03: Seg Response
CoN SDO Data
0x0006	Data	BYTE[N-4]	对象的数据部分*/
int nc_fsm_coe_up_seg_response(
    nc_fsm_coe_t *fsm, /**< Finite state machine. */
    uint8 *datagram    /**< Datagram to use. */
)
{
    nc_slave_t *slave = fsm->slave;
    uint8 *data, mbox_prot;
    uint16 rec_size, data_size;
    nc_sdo_request_t *request = fsm->request;

    data = nc_slave_res_channel_fetch(slave, datagram, &mbox_prot, &rec_size);

    // printf("Upload segment response:\n");

    if (mbox_prot != 0x01)
    { // CoE
        printf("Slave %d: Received mailbox protocol 0x%02X as response.\n", slave->ring_position, mbox_prot);
        return 2;
    }

    if (nc_fsm_coe_check_emergency(fsm, data, rec_size))
        return 2;

    if (rec_size < 4)
    {
        printf("Slave %d: Received currupted SDO upload segment response!\n", slave->ring_position);
        return 2;
    }

    if (NC_READ_U8(data) == 0x3 && // SDO Upload
        NC_READ_U8(data + 1) == 0x4)
    { // abort SDO transfer request
        printf("Slave %d: SDO upload 0x%04X:%02X aborted.\n", slave->ring_position, request->index, request->subindex);
        request->abort_code = NC_READ_U32(data + 6);
        nc_canopen_abort_msg(slave, request->abort_code);
        request->_errno = IO;
        return 2;
    }

    if (NC_READ_U8(data) != 0x3 || // SDO response
        NC_READ_U8(data + 1) != 0x3)
    { // upload segment response
        printf("Slave %d: Invalid SDO upload segment response!\n", slave->ring_position);
        return 2;
    }

    data_size = rec_size - 4; /* Header of segment upload is smaller than
                                 normal upload */

    if (request->data_size + data_size > fsm->complete_size)
    {
        printf("Slave %d: SDO upload 0x%04X:%02X failed: Fragment exceeding complete size!\n",
               slave->ring_position, request->index, request->subindex);
        request->_errno = OVERFLOW;
        return 2;
    }

    memcpy(request->data + request->data_size, data + 2, data_size); // 这么看request->data_size像是一个request->data的偏移量
    request->data_size += data_size;

    if (request->data_size < fsm->complete_size)
    {
        printf("upload seg successful!\n");
        printf("Slave %d: SDO data incomplete (%d / %d). Segmenting...\n", slave->ring_position, data_size, fsm->complete_size);
        //            ec_fsm_coe_up_prepare_segment_request(fsm, datagram);
    }
    printf("upload seg successful!\n");
    return 1;
}