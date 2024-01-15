#include "gba_bitfield_macros.h"
#include "gba_def.h"
#include "gba_types.h"

const u32_t _G_snd_rates[12]=
{
    8013, 7566, 7144, 6742, // C , C#, D , D#
    6362, 6005, 5666, 5346, // E , F , F#, G
    5048, 4766, 4499, 4246  // G#, A , A#, B
};


void DMG_Channel_Enable(u16_t channel, u16_t l_enable, u16_t r_enable) {
  DMG_Snd_Ctl_t snd = REG_SND_DMG_CNT;
  switch (channel) {
    case 1:
      snd.square_left1 = l_enable;
      snd.square_right1 = r_enable;
      REG_SND_DMG_CNT = snd;
      return;
    case 2:
      snd.square_left2 = l_enable;
      snd.square_right2 = r_enable;
      REG_SND_DMG_CNT = snd;
      return;
    case 3:
      snd.wave_left = l_enable;
      snd.wave_right = r_enable;
      REG_SND_DMG_CNT = snd;
      return;
    case 4:
      snd.noise_left = l_enable;
      snd.noise_right = r_enable;
      REG_SND_DMG_CNT = snd;
    default:
      return;
  }

}

void DMG_Set_Volume_Left(u16_t lvl) {
  DMG_Snd_Ctl_t snd = REG_SND_DMG_CNT;
  snd.left_vol = lvl;
  REG_SND_DMG_CNT = snd;
}

void DMG_Set_Volume_Right(u16_t lvl) {
  DMG_Snd_Ctl_t snd = REG_SND_DMG_CNT;
  snd.right_vol = lvl;
  REG_SND_DMG_CNT = snd;
}

void DMG_Set_Volume(u16_t lvl) {
  DMG_Snd_Ctl_t snd = REG_SND_DMG_CNT;
  snd.left_vol = lvl;
  snd.right_vol = lvl;
  REG_SND_DMG_CNT = snd;
}

void DMG_Set_Volume_Ratio(u16_t ratio) {
  DSound_Ctl_t snd = REG_SND_DS_CNT;
  ratio&=3;
  if (ratio^(ratio>>1))
    ratio = 2;
  snd.dmg_vol_ratio = ratio;
  REG_SND_DS_CNT = snd;
}

void Play_Note(Snd_Note_Idx_t note, int octave) {
  Snd_SqW_Freq_Ctl_t snd = {.rate = SND_RATE(note, octave), .reset = 1};
  REG_SND_FREQ(1) = (Snd_Freq_Ctl_t){.dmg_square = snd}; 
}

void DMG_Square1_Init(u16_t duty_cycle, bool_t envelope_increasing, u16_t len, 
    u16_t init_val) {
  REG_SND1_CNT = (Snd_Square_Ctl_t) {
    .duty_cycle = duty_cycle,
    .env_inc = envelope_increasing,
    .env_step_time = len,
    .env_init_val = init_val,
  };
  REG_SND_FREQ(1) = (Snd_Freq_Ctl_t){
    .dmg_square = (Snd_SqW_Freq_Ctl_t){
      .rate = 0, .reset = 0, .timed = 0
    },

  };
}



