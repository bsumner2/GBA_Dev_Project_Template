#include "gba_bitfield_macros.h"
#include "gba_def.h"
#include "gba_functions.h"
#include "gba_types.h"
#include <stddef.h>

#if 0

#include "porkys_porkies.h"

void setup_timer(int timer, Timer_Handle_t *hndl);

void setup_sounda(void) {
  MASTER_SOUND_ENABLE;
  REG_DISPLAY_CNT_SET_MODES(DCNT_V_MODE3, DCNT_BG_MODE2);
  DSound_Ctl_t ctl = (DSound_Ctl_t){
    .dmg_vol_lvl = 2, .a_lvl = 1, .a_timer_no = 0, .a_left_enable = 1, .a_right_enable = 1,
      .a_fifo_reset = 1,
  };
  REG_SND_DS_CNT = ctl;
}

int main(void) {
  
  setup_sounda();
  VIDEO_BUF[0] = 0x7fff;
  DMA_Memcpy2((void*)FIFO_A_BUF, &porkys_porkies, 1, 0, 
      DMA_ADJ_FIXED, DMA_ADJ_INC, 1, 1, DMA_SYNC_TO_TIMER, false);
  Timer_Handle_t timer_hndl;
  //u16_t tickrate = (1<<16) - (16780000/porkys_porkies_sample_rate);
  u16_t tickrate = (1<<16) - (16777216/porkys_porkies_sample_rate);
  timer_hndl.data = tickrate;
  timer_hndl.controller.enable_flag = 1;
  timer_hndl.controller.interrupt_flag = 0;
  timer_hndl.controller.cascade_mode_flag = 0;
  timer_hndl.controller.frequency = 0;
  //SET_RESAMPLE_RES;
  setup_timer(0, &timer_hndl);

  int i = 0;
  while (1) {
    VIDEO_BUF[0] = ++i&0x7FFF;
  } 
}

#elif 1
int main(void) {
  MASTER_SOUND_ENABLE;
  DMG_Channel_Enable(1, true, true);
  DMG_Set_Volume(7);
  DMG_Set_Volume_Ratio(SND_DMG_RATIO_100); 

  DMG_Square1_Init(SND_SQUARE_DUTY_CYCLE_1_2ND, false, 7, 12);

  while (1) {
    if (KEY_PRESSED(A)) {
      Play_Note(NOTE_A, 5);
    }
  }
  
}

#else

u16_t colors[256] = {0};
int i=0;

void irqcb(void) {
  
  PAL_BG_BUF[i++] = 0x7FFF;
}

void Mode4_Plot(int x, int y, u8_t pal_id) {
  u16_t *location = &VIDEO_BUF[(SCREEN_WIDTH>>1)*y + (x>>1)], tmp;
  if (x&1) {
    tmp = 0x00FF&*location;
    *location = (pal_id<<8)|tmp;
  } else {
    tmp = 0xFF00&*location;
    *location = pal_id|tmp;
  }
}


void setup_timer_test_dpy(void) {
  REG_DISPLAY_CNT_SET_MODES(DCNT_V_MODE4, DCNT_BG_MODE2);
  u32_t pal_idx = 0;
  for (int i = 0; i < SCREEN_HEIGHT; ++i)
  for (int j = 0; j < SCREEN_WIDTH; ++j)
    Mode4_Plot(j, i, pal_idx++&0xFF);  
}




int main(void) {
  setup_timer_test_dpy();

  Timer_Handle_t tm = {

    .data = (u16_t)(-0x4000),
    .controller = {
      .frequency = 0,
      .cascade_mode_flag = 0,
      .interrupt_flag = 1,
      .enable_flag = 1,
    },
  };

  IRQ_Init(ISR_Master_ctl);
  IRQ_Add(IRQ_TIMER0, irqcb);
  setup_timer(0, &tm);


  while (1) {
    
  }

  
}
  

#endif
