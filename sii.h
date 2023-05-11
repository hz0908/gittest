#ifndef SII
#define SII
#include "type.h"
void sii_fetch(nc_slave_t *slave);//获取从站应用信息区数据
void init_slave_basic_info(nc_slave_t *slave);//slave_basic_info初始化
void pdo_entry_parameters_set(nc_slave_t *slave);//将获取的数据设置到主站
#endif // SII






