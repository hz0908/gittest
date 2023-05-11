#ifndef SDO
#define SDO
#include "type.h"

// 终止代码和描述
struct nc_code_msg
{
    uint32 code;
    const char *message;
};
typedef struct nc_code_msg nc_code_msg_t;
/*Network Configuration - Finite State Machine for CANopen Object Dictionary Entry".*/
// 网络配置状态机
struct nc_fsm_coe
{
    nc_slave_t *slave; /**< slave the FSM runs on */
    dnode *sdo;
    uint16 index;
    uint8 subindex;            /**< current subindex */
    nc_sdo_request_t *request; /**< SDO request */
    uint32 offset;             /**< Data offset during segmented download. */
    uint32 complete_size;
    uint32 remaining;    /**< Remaining bytes during segmented download. */
    uint16 segment_size; /**< Current segment size. */
};
typedef struct nc_fsm_coe nc_fsm_coe_t; /**< \see ec_fsm_coe */

int Sdo_Info_Get(nc_slave_t *slave);
int nc_fsm_coe_dict_start(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_prepare_dict(uint8 *datagram);
bool service_channel_request_by_ring_position(uint16 ring_position, uint16 size, uint8 *data, uint16 len_ch);
int nc_fsm_coe_dict_response(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_check_emergency(nc_fsm_coe_t *fsm, const uint8 *data, uint16 size);
void nc_canopen_abort_msg(const nc_slave_t *slave, uint32 abort_code);
void desc_func(nc_slave_t *slave, nc_fsm_coe_t *fsm);
int nc_fsm_coe_dict_prepare_desc(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_dict_desc_response(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_dict_prepare_entry(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_dict_entry_response(nc_fsm_coe_t *fsm, uint8 *datagram);

int SDO_Download(nc_slave_t *slave, uint8 *data, uint16 index, uint8 subindex, uint8 number, uint8 length);
int nc_fsm_coe_down_start(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_prepare_down_start(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_down_response(nc_fsm_coe_t *fsm, uint8 *datagram);
void nc_fsm_coe_down_prepare_segment_request(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_down_seg_response(nc_fsm_coe_t *fsm, uint8 *datagram);

int SDO_Upload(nc_slave_t *slave, uint8 *data, uint16 index, uint8 subindex, uint8 number);
int nc_fsm_coe_up_start(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_prepare_up(nc_fsm_coe_t *fsm, uint8 *datagram);
int nc_fsm_coe_up_response(nc_fsm_coe_t *fsm, uint8 *datagram);
void nc_fsm_coe_up_prepare_segment_request(uint8 *datagram);
int nc_fsm_coe_up_seg_response(nc_fsm_coe_t *fsm, uint8 *datagram);
#endif // SDO
