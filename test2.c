#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datagram.h"
#include "frame.h"
#include "format.h"



// 测试程序
int main()
{

 nc_frame_t frm;
 initframe(&frm);
 uint8 data[2]={0x19,0x10};
  //regAccessdgrmAdd(&frm,APRD,1,1,sizeof(uint16),data,0);
  //regAccessdgrmAdd(&frm,APRD,1,1,sizeof(uint16),data,0);
  //int16 chAccessdgrmAdd(nc_frame_t *targetFrame,uint8 cmd,uint16 st,\
                            uint16 offset,uint16 datalen,uint8 ch,uint8* data,uint16 irq );
  
  chAccessdgrmAdd(&frm,0x20,0,0x10,2,1,data,0);
  chAccessdgrmAdd(&frm,0x20,0,0x3423,3,2,data,0);
  FrameTransmit(&frm);


return 0;
}




