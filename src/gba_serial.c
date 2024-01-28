#include "gba_def.h"
#include "gba_bitfield_macros.h"
#include "gba_functions.h"
#include "gba_types.h"

bool_t Serial_MP_Setup(bool_t initialize, bool_t *is_parent) {

  Serial_Ctl_t sctl = REG_SIOCNT;
  if (initialize) {
    SERIAL_RCNT_INIT;
    sctl.multi.pad0 = SERIAL_MP_PAD0_REQUIRED_VALUE;
    REG_SIOCNT = sctl;
  }
  
  
  sctl.raw_bits = REG_SERIAL_IOCNT;
  if (is_parent)
    *is_parent = sctl.multi.SI_terminal;
  return sctl.multi.SD_terminal;
}

Serial_MP_Rsp_t Serial_MP_Get_Response(void) {
  Serial_MP_Rsp_t data;
  for (int i = 0; i < 4; ++i)
    data.data[i] = SIO_MULTI(i);
  Serial_Ctl_t sctl = REG_SIOCNT;
  data.player_id = sctl.multi.mp_ID;
  return data;
}



