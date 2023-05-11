#ifndef CHANNEL_CONFIG_PARAMETER
#define CHANNEL_CONFIG_PARAMETER
#include "type.h"

#define CH_CONTROL_MI    0x03//中断允许，主写从读
#define CH_CONTROL_SI    0x13//中断允许，从写主读
#define CH_CONTROL_MN     0x00//中断未允许，主写从读
#define CH_CONTROL_SN     0x10//中断未允许，从写主读
#define CH_CONTROL_I     0x03//中断允许
#define CH_CONTROL_N    0x00//中断未允许

#endif // CHANNEL_CONFIG_PARAMETER

