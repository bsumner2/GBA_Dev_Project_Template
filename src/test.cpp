#include "gba_def.h"
#include "gba_functions.h"
int main(void) {
  for (int i = 0; i < SCREEN_HEIGHT; ++i)
    for (int j = 0; j < 120; ++j)
      VIDEO_BUF[i*120 + j] = 0;
  PAL_BG_BUF[1] = 0x7FFF;
  PAL_BG_BUF[0] = 0;
  {
    REG_DISPLAY_STAT |= (1U<<3);
    REG_IE |= 1<<IRQ_Idx_t::IRQ_VBLANK;
    REG_ISR_MAIN = ISR_master_ctl;
    REG_IME = 1;
  }
  REG_DISPLAY_CNT_SET_MODES(DCNT_BG_MODE2, DCNT_V_MODE4);

  while (1);


}
