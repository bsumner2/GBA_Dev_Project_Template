#ifndef _GBA_DEF_H_
#define _GBA_DEF_H_


#include "gba_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* extern "C" name mangler guard */

#define SCREEN_WIDTH      240
#define SCREEN_HEIGHT     160

#define MEM_BIOS_CTLS     0x03000000
#define MEM_IO            0x04000000
#define MEM_PAL           0x05000000
#define MEM_VRAM          0x06000000
#define MEM_OAM           0x07000000
#define MEM_GAMEPAK_SRAM  0x0E000000

#define PAL_BG_MEMBLOCK_LEN   0x00200
#define PAL_OBJ_MEMBLOCK_LEN  0x00200

#define MEM_PAL_OBJ (MEM_PAL + PAL_BG_MEMBLOCK_LEN)

#define PAL_BG_BUF  ((u16_t*) MEM_PAL)
#define PAL_OBJ_BUF ((u16_t*) MEM_PAL_OBJ)





#define REG_DISPLAY_CNT *((vu32_t*) MEM_IO)
#define REG_DISPLAY_STAT *((vu16_t*) (MEM_IO + 0x0004))
#define REG_DISPLAY_VCOUNT *((vu16_t*) (MEM_IO + 0x0006))
#define SRAM_BUF ((u8_t*) (MEM_GAMEPAK_SRAM))


#define BG_AFFINE_PARAMS_MEM_OFFSET 0x0000
#define BG_CNT_MEM_OFFSET           0x0008
#define BG_HOFS_MEM_OFFSET          0x0010
#define BG_VOFS_MEM_OFFSET          0x0012

#define REG_BG_AFFINE_PARAMS \
  ((BgAffine_t*) (MEM_IO + BG_AFFINE_PARAMS_MEM_OFFSET))

#define REG_BG_CNT ((vu16_t*) (MEM_IO + BG_CNT_MEM_OFFSET))
#define REG_BG_CONTROLLER ((Bg_Controller_t*) (MEM_IO + BG_CNT_MEM_OFFSET))

#define REG_BG_OFS ((Coordinate*) (MEM_IO + BG_HOFS_MEM_OFFSET))
#define REG_BG_HOFS(bgno) *((vu16_t*) (MEM_IO + BG_HOFS_MEM_OFFSET + (bgno<<2)))
#define REG_BG_VOFS(bgno) *((vu16_t*) (MEM_IO + BG_VOFS_MEM_OFFSET + (bgno<<2)))


#define BG_SCR_BLOCK_BUF ((ScreenBlock*) MEM_VRAM)

#define VIDEO_BUF ((u16_t*) MEM_VRAM)
#define TILE_BUF ((TBlock*) MEM_VRAM)
#define SPR_TILE_BUF (TILE_BUF + 4)
#define SPR_TILE8_BUF ((TBlock8*) MEM_VRAM)

#define OAM_BUF       ((Obj_Attr_t*) MEM_OAM)
#define OAM_AFF_BUF   ((Obj_Affine_t*) MEM_OAM)


#define REG_KEYINPUT *((vu32_t*) (MEM_IO + 0x0130))

#define REG_DMA ((volatile DMA_Handle_t*) (MEM_IO + 0x00B0))

#define REG_TM ((volatile Timer_Handle_t*) (MEM_IO + 0x0100))
#define REG_TM_CNT ((volatile Timer_Controller_t*) (MEM_IO + 0x0102))
#define REG_TM_DATA ((vu16_t*) (MEM_IO + 0x0100))


#define REG_IME *((vu16_t*) (MEM_IO + 0x0208))

#define REG_IE  *((vu16_t*) (MEM_IO + 0x0200))
#define REG_IF  *((vu16_t*) (MEM_IO + 0x0202))
#define REG_IFBIOS *((vu16_t*) (MEM_BIOS_CTLS + 0x7FF8))

#define REG_IE_FIELDS *((volatile Interrupt_Enabler_t*) (MEM_IO + 0x0200))
#define REG_IF_FIELDS *((volatile Interrupt_Enabler_t*) (MEM_IO + 0x0202))
#define MEM_ISR_CALLBACK_ADDR 0x03007FFC

#define REG_ISR_MAIN *((IRQ_Callback_t*) (MEM_BIOS_CTLS + 0x7FFC))


#define FIFO_A_BUF ((vu8_t*) (MEM_IO + 0x00A0))
#define FIFO_B_BUF ((vu8_t*) (MEM_IO + 0x00A4))

#define REG_SND_DMG_CNT *((volatile DMG_Snd_Ctl_t*) (MEM_IO + 0x0080))
#define REG_SND_DS_CNT *((volatile DSound_Ctl_t*) (MEM_IO + 0x0082))
#define REG_SND_STAT *((vu16_t*) (MEM_IO + 0x0084))

// NOTE FROM Sound Register Documentation on http://belogic.com/gba 
// The BIAS setting (bits 9-0 of REG_SOUNDBIAS @ 0x04000088 is used to offset 
// the sound output and bring it back into asigned range. When the BIOS starts 
// up, it runs a timing loop where it slowly raises the BIAS voltage from 0 to 
// 512. This setting should not be changed. 
// At best, the sound will become distorted. 
// At worst the amplifier inside the GBA could be damaged. 
// When accessing bits FE, a read-modify-write is required.
// ------------------------------------------------------------------
// Due to the lack of documentation on this register, along with the warnings
// about how changing bits 9-0 can damage the actual speaker hardware, I am only
// providing this macro that sets bits FE, which are the resample resolution
// bits. According to belogic, 0b01 is the most common setting for bits FE, 
// hence why the bias register is being OR'd with 0x4000
#define SET_RESAMPLE_RES \
  do *((vu16_t*) (MEM_IO + 0x0088)) |= 0x4000; while (0)

#define REG_SND1_SWEEP *((volatile Snd1_Sweep_Ctl_t*) (MEM_IO + 0x0060))

#define REG_SND_FREQ(dmg_channel) \
  *((volatile Snd_Freq_Ctl_t*) (MEM_IO + 0x0064 + (((dmg_channel-1)&3)<<3)))

#define REG_SND1_CNT *((volatile Snd_Square_Ctl_t*) (MEM_IO + 0x0062))
#define REG_SND2_CNT *((volatile Snd_Square_Ctl_t*) (MEM_IO + 0x0068))

#define REG_SND3_BANK_CNT *((volatile Snd3_Bank_Ctl_t*) (MEM_IO + 0x0070))
#define REG_SND3_CNT *((volatile Snd3_Wave_Ctl_t*) (MEM_IO + 0x0072))
#define REG_WAVERAM ((vu32_t*) (MEM_IO + 0x0090))

#define REG_SND4_CNT *((volatile Snd4_Noise_Ctl_t*) (MEM_IO + 0x0078))
#ifdef __cplusplus
}
#endif  /* extern "C" name mangler guard */

#endif  /* _GBA_DEF_H_ */
