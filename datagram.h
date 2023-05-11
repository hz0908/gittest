#ifndef DATAGRAM
#define DATAGRAM
#include "type.h"
#include "frame.h"


#define APRD 	0x10
#define APWR 	0x11
#define FPRD	0x12
#define FPWR 	0x13
#define BRD 	0x14
#define BWR 	0x15

/**通道访问报文读写命令**/
#define ACHRD 0x20
#define ACHWR 0x21
#define FCHRD 0x22
#define FCHWR 0x23

/**单圈/双圈映射报文命令**/
#define SINGLECYCLEMAP 0x26
#define DOUBLECYCLEMAP 0x27
/**产生时间戳报文命令**/
#define TSCMD		0x30
/*同步帧/同步复位帧*/
#define SYNCRESETCMD 	0x32
#define SYNCCOMMAND		0x33
/**32/64位分布时钟补偿报文命令**/
#define DC32COMMAND	0x34
#define DC64COMMAND	0x36

int16 regAccessdgrmAdd(nc_frame_t* targetFrame,uint8 cmd,uint16 st,uint16 offset,\
                                                uint16 datalen,uint8* data,uint16 irq);

int16 chAccessdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 st,\
                            uint16 offset,uint16 datalen,uint8 ch,uint8* data,uint16 irq );

int16 chMapdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 mapNo,uint16 datalen,\
                                                        uint8 *data,uint16 irq  ,uint16 mfmo );

int16 timesdgrmAdd(nc_frame_t *targetFrame);

int16 syncdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint8* data);

int16 dclkdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint8* data);

//bool datagrmsIsErr(const uint8 *datagrams);


#endif // DATAGRAM

