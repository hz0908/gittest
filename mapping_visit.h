#include "type.h"
#include "register.h"
#include "frame.h"
#include <string.h>

/***通道映射读写函数***/
bool mapping_singlecycle_wr(uint16 MAPNo, uint16 len, uint8* data);
bool mapping_singlecycle_rd(uint16 MAPNo, uint16 len, uint8* data);
bool mapping_doublecycle(uint16 MAPNo, uint16 len, uint8 *data);

/***打印函数***/
void printData(const uint8* data, uint16 len) ;