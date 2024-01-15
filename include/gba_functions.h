#ifndef _GBA_FUNCTIONS_H_
#define _GBA_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif  /* C++ Name Mangler guard */

#include "gba_def.h"
#include "gba_types.h"
#include "gba_bitfield_macros.h"
#include <stdarg.h>

/** 
 * @brief Sets video and background mode flags for REG_DISPLAY_CNT. Avoid using 
 * this raw variadic function. Use the macro, REG_DISPLAY_CNT_SET_MODES(...).
 * @details If you insist against using the macro, just add 0xFFFFFFFF as the 
 * final param when calling this function, as it's what signals to this function
 * when to stop stepping through the va_list.
 * @params Variadic: List of all mode flags you would like to set.
 * */
void _Internal_DisplayControl_SetModes(u32_t mode1, ...);
#define REG_DISPLAY_CNT_SET_MODES(...) \
  _Internal_DisplayControl_SetModes(__VA_ARGS__, 0xFFFFFFFF)

/** 
 * @brief Write (amount: len) bytes of (src: data) to 
 * (destination: &SRAM_BUF[offset])
 * */
void SRAM_Write(const void* data, u16_t offset, u16_t len);

/** 
 * @brief Read (amount: len) bytes of (src: &SRAM_BUF[offset]) and copy it to
 * (destination: dest).
 * */
void SRAM_Read(void* dest, u16_t offset, u16_t len);

/**
 * @brief Initialize OAM Buffer 
 * */
void OAM_Init(Obj_Attr_t* obj_buf, u32_t obj_ct);
/** 
 * @brief Copy (amount: ct) Obj Attribute struct instances to dest from src.
 */
  void OAM_Copy(Obj_Attr_t* dest, const Obj_Attr_t* src, u32_t ct);

// !!! TODO: FINISH DOCUMENTATION!

/** 
 * @brief Initialize object attributes for a given Object's Attributes struct.
 * @param obj Object Attributes struct to be init'd.
 * @param object_mode Signifies the mode to display the object in (i.e.: normal,
 * affine, or hidden).
 * ...
 * */
Obj_Attr_t* ObjAttr_Init(Obj_Attr_t* obj, u16_t object_mode, 
    u16_t graphics_mode, ObjectDims dims, u16_t x, u16_t y, bool_t mosaic,
    bool_t depth_8bpp, u16_t palette_idx, u16_t priority, u16_t tile_idx, 
    u16_t affine_idx);

/** 
 * */
Obj_Attr_t* ObjAttr_QuickInit(Obj_Attr_t* obj, ObjectDims dims, u16_t pal_idx, 
    u16_t tile_idx, bool_t depth_8bpp);

void BgCnt_Init(Bg_Controller_t* cnt, u16_t priority,
    u16_t tiles_offset, bool_t is_mosaic, bool_t is_8bpp, u16_t scr_ent_offset,
    bool_t affine_wrap, u16_t background_size);

void BgOffset_Set(u16_t bgno, Coordinate coord);

void* DMA_QuickMemcpy(void* dest, const void* src, u32_t dma_slot, 
    DMA_Controller_t ctl);


void* DMA_Memcpy2(void* dest, const void* src, u32_t dma_slot, 
    u16_t chunk_ct, u16_t dest_adj_mode, u16_t src_adj_mode, bool_t repeat,
    bool_t word_len_chunks, u16_t timing_mode, bool_t interrupt_after);



IWRAM_CODE void ISR_Master_ctl(void);

void IRQ_Init(IRQ_Callback_t isr_main_callback);
IRQ_Callback_t IRQ_SetMaster(IRQ_Callback_t new_master_isr_cb);
IRQ_Callback_t IRQ_Add(IRQ_Idx_t, IRQ_Callback_t);
IRQ_Callback_t IRQ_Rm(IRQ_Idx_t);
IRQ_Callback_t IRQ_Set(IRQ_Idx_t type, IRQ_Callback_t cb, int opts);

void IRQ_Enable(IRQ_Idx_t);
void IRQ_Disable(IRQ_Idx_t);


void setup_timer(int timer, Timer_Handle_t *hndl);



void DMG_Channel_Enable(u16_t channel, u16_t l_enable, u16_t r_enable);

void DMG_Set_Volume_Left(u16_t lvl);
void DMG_Set_Volume_Right(u16_t lvl);

void DMG_Set_Volume(u16_t lvl);
void DMG_Set_Volume_Ratio(u16_t ratio);

void Play_Note(Snd_Note_Idx_t note, int octave);
void DMG_Square1_Init(u16_t duty_cycle, bool_t envelope_increasing, u16_t len,
    u16_t init_val);

#ifdef __cplusplus
}
#endif  /* C++ Name Mangler guard */

#endif  /* _GBA_FUNCTIONS_H_ */
