#include "gba_def.h"
#include "gba_functions.h"

void swappal(void) {
  u16_t tmp = PAL_BG_BUF[0];
  PAL_BG_BUF[0] = PAL_BG_BUF[1];
  PAL_BG_BUF[1] = tmp;
}

int main(void) {
  IRQ_Init(ISR_Master_ctl);
  
  IRQ_Add(IRQ_Idx_t::IRQ_VBLANK, swappal);
  REG_DISPLAY_CNT_SET_MODES(DCNT_BG_MODE2, DCNT_V_MODE4);
  PAL_BG_BUF[0] = 0;
  PAL_BG_BUF[1] = 0x7FFF;

  while (1);


}
