#ifndef _GBA_SERIAL_H_
#define _GBA_SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif  /* CXX Header Guard */
#include "gba_def.h"
#include "gba_util_macros.h"

STAT_INLN void Serial_MP_SetData(u16_t data) {
  SIO_MULTI_SEND = data;
}

STAT_INLN void Serial_MP_StartTransfer(void) {
  Serial_Ctl_t sctl = REG_SIOCNT;
  sctl.multi.start_bit = 1;
}

Serial_MP_Rsp_t Serial_MP_Get_Response(void);

#ifdef __cplusplus
}
#endif  /* CXX Header Guard */

#endif  /* _GBA_SERIAL_H_ */

