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


