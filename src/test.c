#include "gba_bitfield_macros.h"
#include "gba_def.h"
#include "gba_functions.h"
#include "gba_inlines.h"
#include "gba_serial.h"
#include "gba_types.h"
#include <stddef.h>
#include <stdio.h>

#define GBA_STDIO_LEGACY
#include "gba_stdio.h"

#if 0

#include "porkys_porkies.h"

void setup_timer(int timer, Timer_Handle_t *hndl);

void setup_sounda(void) {
  MASTER_SOUND_ENABLE;
  DCNT_SET_MODES(DCNT_V_MODE3, DCNT_BG_MODE2);
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

#elif 0
int d_major[] = {
  NOTE_C,
  NOTE_D,
  NOTE_E,
  NOTE_F,
  NOTE_G,
  NOTE_A,
  NOTE_B,
};

int main(void) {
  MASTER_SOUND_ENABLE;
  DMG_Channel_Enable(1, true, true);
  DMG_Set_Volume(7);
  DMG_Set_Volume_Ratio(SND_DMG_RATIO_100); 

  DMG_Square1_Init(SND_SQUARE_DUTY_CYCLE_1_2ND, false, 7, 12);
  int curr = 0;
  int octave = -1;
  while (1) {
    if (KEY_PRESSED(A)) {
      while (KEY_PRESSED(A)) continue;
      if (!(curr&7))
        ++octave;
      Snd_Note_Idx_t current = d_major[curr++&7];

      // TODO figure out why tf starting note just turns to scheisse
      // after next octave
    }
  }
  
}
#elif 1
bool_t is_host = false;
int main(void) {
  DCNT_SET_MODES(DCNT_V_MODE3, DCNT_BG_MODE2);
  while (1) {
    if (KEY_PRESSED(A)) {
      while (KEY_PRESSED(A)) 
        continue;
      Mode3_printf(0,64, 0x7FFF, "Initializing multiplayer.\n");
      if (!Serial_MP_Setup(SERIAL_BAUD_FLAG_9600, true, &is_host))
        while (!Serial_MP_Setup(SERIAL_BAUD_FLAG_9600, false, &is_host));
      break;
    }
  }

  Mode3_printf(0, 124, 0x7fff, "Successful.\nIs Host: %s\n", is_host?"True":"False");
  Serial_MP_Rsp_t response;
  Serial_Ctl_t ctl;
  while (1) {
    if (!KEY_PRESSED(A)) continue;
    if (is_host) {
      Serial_MP_SetData(0x7fff);
      Serial_MP_StartTransfer();
      while (((ctl = REG_SIOCNT).multi.start_bit))
        Mode3_puts("Awaiting", 0, 152, 0x7FFF);
    } else {
      Serial_MP_SetData(RED_MASK);
      while (((ctl = REG_SIOCNT).multi.start_bit))
        Mode3_puts("Awaiting", 0, 152, 0x7FFF);
    }
    break;
  }
  if (!((ctl = REG_SIOCNT).multi.SD_terminal) || ctl.multi.mp_error) {
    Mode3_printf(0, 0, RED_MASK, "Error occurred: %X", ctl.raw_bits);
  }
  Serial_MP_Get_Response(&response);


  Mode3_printf(0, 5, 0x7fff, "Player ID: %d\nIs Master: %s", response.player_id, is_host?"True":"False");
  Mode3_printf(0, 80, 0x7fff, "SERIAL_MULTI<1,2> = 0x%04X, 0x%04X", response.data[0], response.data[1]);
  while (1);
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
  DCNT_SET_MODES(DCNT_V_MODE4, DCNT_BG_MODE2);
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
