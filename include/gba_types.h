#ifndef _GBA_TYPES_H_
#define _GBA_TYPES_H_


#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdbool.h>
#include <stdint.h>
#endif  /* extern "C" name mangler guard */

#include "gba_util_macros.h"


typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long int u64_t;

typedef signed char s8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long int s64_t;

typedef uintptr_t memaddr_t;

typedef volatile u8_t vu8_t;
typedef volatile u16_t vu16_t;
typedef volatile u32_t vu32_t;
typedef u8_t bool_t;

typedef struct Coord Coordinate;
typedef struct _4BPP_Tile Tile, Tile4;
typedef struct _8BPP_Tile Tile8;


struct Coord {
  s16_t x, y;
} ALIGN(4);

struct _4BPP_Tile {
  u32_t buf[8];
};

struct _8BPP_Tile {
  u32_t buf[16];
};


typedef Tile TBlock[512];
typedef Tile8 TBlock8[256];

typedef struct OAM_Attr0_Bitfield ObjAttr0_t;
typedef struct OAM_Regular_Attr1_Bitfield RegAttr1_t;
typedef struct OAM_Affine_Attr1_Bitfield AffineAttr1_t;
typedef struct OAM_Attr2_Bitfield ObjAttr2_t;
typedef union OAM_Regular_Affine_Attr1_Union ObjAttr1_t;
typedef struct OAM_Affine_Matrix_MemMap  Obj_Affine_t;
typedef struct OAM_Attributes_MemMap Obj_Attr_t;
typedef struct OAM_RegAff_Shared_Attr1 SharedAttr1_t;

struct OAM_Attr0_Bitfield {
  u16_t y_coord : 8;
  u16_t object_mode : 2;
  u16_t graphics_mode : 2;
  u16_t mosaic_flag : 1;  /// Set high for mosaic effect.
  u16_t color_depth8 : 1;  /// Set high for 8bpp otherwise 4bpp
  u16_t shape : 2;
} ALIGN(2);

struct OAM_Regular_Attr1_Bitfield {
  u16_t x_coord : 9;
  u16_t pad0 : 3;
  u16_t horizontal_flip : 1;
  u16_t vertical_flip : 1;
  u16_t size : 2;
} ALIGN(2);

struct OAM_Affine_Attr1_Bitfield {
  u16_t x_coord : 9;
  u16_t affine_idx : 5;
  u16_t size : 2;
} ALIGN(2);

struct OAM_RegAff_Shared_Attr1 {
  u16_t x_coord : 9;
  u16_t pad0 : 5;
  u16_t size : 2;
};

union OAM_Regular_Affine_Attr1_Union {
  SharedAttr1_t shared_attributes;
  RegAttr1_t regular;
  AffineAttr1_t affine;
};

struct OAM_Attr2_Bitfield {
  u16_t tile_idx : 10;
  u16_t priority : 2;
  u16_t pal_idx : 4;  /// Ignored if in 8bpp mode set by ObjAttr0_t.color_depth8
} ALIGN(2);

struct OAM_Affine_Matrix_MemMap {
  u16_t pad0[3];
  s16_t pa;
  u16_t pad1[3];
  s16_t pb;
  u16_t pad2[3];
  s16_t pc;
  u16_t pad3[3];
  s16_t pd;
} ALIGN(4);

struct OAM_Attributes_MemMap {
  ObjAttr0_t attr0;
  ObjAttr1_t attr1;
  ObjAttr2_t attr2;
  s16_t pad1;
} ALIGN(4);

typedef struct BG_Affine_Params_t BgAffine_t;
typedef struct Bg_Controller Bg_Controller_t;
typedef u16_t ScreenBlock[1024];

struct Bg_Controller {
  u16_t priority : 2;
  u16_t tblock_offset : 2; 
  u16_t pad0 : 2;
  u16_t mosaic_flag : 1;
  u16_t color_depth8 : 1;
  u16_t scrblock_offset : 5;
  u16_t affine_wrap_flag : 1;
  u16_t size : 2;
} PACKED;

struct BG_Affine_Params {
    s16_t pa, pb;
    s16_t pc, pd;
    s32_t dx, dy;
} ALIGN(4);

typedef struct DMA_Ctl DMA_Controller_t;
typedef struct DMA_Hndl DMA_Handle_t;

struct DMA_Ctl {
  u16_t transfer_ct : 16;
  u16_t pad0 : 5;
  u16_t dest_adj : 2;
  u16_t src_adj : 2;
  u16_t repeat_flag : 1;
  u16_t chunk_size32 : 1;
  u16_t pad1 : 1;
  u16_t timing_mode : 2;
  u16_t interrupt_flag : 1;
  u16_t enable_flag : 1;
} ALIGN(4);


struct DMA_Hndl {
  const void* src;
  void* dest;
  DMA_Controller_t controller;
};

typedef struct Timer_Ctl Timer_Controller_t;
typedef struct Timer_Hndl Timer_Handle_t;

struct Timer_Ctl {
  u16_t frequency : 2;
  u16_t cascade_mode_flag : 1;
  u16_t pad0 : 3;
  u16_t interrupt_flag : 1;
  u16_t enable_flag : 1;
  u16_t pad1 : 8;
} ALIGN(2);

struct Timer_Hndl {
  u16_t data;
  Timer_Controller_t controller;
} ALIGN(2);


typedef void (*IRQ_Callback_t)(void);
typedef struct Interrupt_En Interrupt_Enabler_t;

struct Interrupt_En {
  u16_t vblank : 1;
  u16_t hblank : 1;
  u16_t vcount : 1;
  u16_t timer : 4;
  u16_t serial_communication : 1;
  u16_t direct_mem_access : 4;
  u16_t keypad : 1;
  u16_t cartridge : 1;
  u16_t pad0 : 2;
};

typedef struct {
  u32_t flag;
  IRQ_Callback_t isr_cb;
} PACKED IRQ_Entry_t;

typedef enum {
  IRQ_VBLANK,
  IRQ_HBLANK,
  IRQ_VCOUNT,
  IRQ_TIMER0,
  IRQ_TIMER1,
  IRQ_TIMER2,
  IRQ_TIMER3,
  IRQ_SERIAL,
  IRQ_DMA0,
  IRQ_DMA1,
  IRQ_DMA2,
  IRQ_DMA3,
  IRQ_KEYPAD,
  IRQ_GAMEPAK,
  IRQ_IDX_LIM,
} IRQ_Idx_t;

typedef struct snd_dsound_ctl DSound_Ctl_t;

struct snd_dsound_ctl {
  u16_t vol_lvl : 2;
  u16_t a_lvl : 1, b_lvl : 1;
  u16_t pad0 : 4;
  u16_t a_right_enable : 1, a_left_enable : 1;
  u16_t a_timer_no : 1, a_fifo_reset : 1;
  u16_t b_right_enable : 1, b_left_enable : 1;
  u16_t b_timer_no : 1, b_fifo_reset : 1;
} ALIGN(2);


typedef struct snd_dmg_ctl DMG_Snd_Ctl_t;
typedef struct snd1_sweep_ctl Snd1_Sweep_Ctl_t;

typedef struct snd_freq_ctl Snd_Freq_Ctl_t;

typedef struct snd_bank_ctl Snd3_Bank_Ctl_t;
typedef struct snd_wav_ctl Snd3_Wave_Ctl_t;

typedef struct snd_sq_ctl Snd_Square_Ctl_t;
typedef struct snd_envelope_ctl Snd4_Noise_Ctl_t;
typedef struct snd_envelope_ctl Snd_Envelope_Ctl_t;
typedef union snd_shared_env_ctl Snd_SqN_Ctl_t;

typedef struct snd_ns_params Snd4_Param_Ctl_t;

struct snd_dmg_ctl {
  u16_t left_vol :3;
  u16_t pad0 : 1;
  u16_t right_vol : 3;
  u16_t pad1 : 1;
  u16_t square_left1 : 1, square_left2 : 1;
  u16_t wave_left : 1, noise_left : 1;
  u16_t square_right1 : 1, square_right2 : 1;
  u16_t wave_right : 1, noise_right : 1;
} ALIGN(2);

struct snd1_sweep_ctl {
  u16_t shift_amt : 3;
  u16_t increasing : 1;
  u16_t step_time : 3;
  u16_t pad : 9;
} ALIGN(2);


struct snd_envelope_ctl {
  u16_t len : 6;
  u16_t pad0 : 2;
  u16_t env_step_time : 3;
  u16_t env_inc : 1;
  u16_t env_init_val : 4;
} ALIGN(2);

struct snd_sq_ctl {
  u16_t len : 6;
  u16_t duty_cycle : 2;
  u16_t env_step_time : 3;
  u16_t env_inc : 1;
  u16_t env_init_val : 4;
} ALIGN(2);

struct snd_freq_ctl {
  u16_t rate : 11;
  u16_t pad0 : 3;
  u16_t timed : 1;
  u16_t reset : 1;
} ALIGN(2);


struct snd3_bank_ctl {
  u16_t pad0 : 5;
  u16_t bank_mode : 1;
  u16_t bank_idx : 1;
  u16_t enable : 1;
  u16_t pad1 : 8;
} ALIGN(2);

struct snd_wav_ctl {
  u16_t len : 8;
  u16_t pad0 : 5;
  u16_t vol : 3;
} ALIGN(2);

union snd_shared_env_ctl {
  Snd_Envelope_Ctl_t shared_opts;
  Snd4_Noise_Ctl_t noise_opts;
  Snd_Square_Ctl_t square_opts;
} ALIGN(2);

struct snd_ns_params {
  u16_t divider_freq : 3;
  u16_t stage_ct_flag : 1;
  u16_t pre_step_freq : 4;
  u16_t pad0 : 6;
  u16_t timed : 1;
  u16_t reset : 1;
} ALIGN(2);


// GBA Direct Sound




#ifdef __cplusplus
}
#endif  /* extern "C" name mangler guard */

#endif  /* _GBA_TYPES_H_ */
