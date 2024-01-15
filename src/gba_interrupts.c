#include "gba.h"
#include "gba_def.h"
#include "gba_functions.h"
#include "gba_types.h"
#include <stddef.h>

/** Credit for this IRQ Handling switchboard implementation goes to the creator
 * of the Tonc GBA development kit library, Jasper Vijn. 
 *------------------------------------------------------------------------------
 * I wanted to do my own barebones and minimalist version, but after seeing how 
 * elegant his IRQ handler switchboard was at being able to handle any kind of 
 * interrupt and essentially having a lookup table for different callback 
 * functions that correspond to the triggering of a specific IRQ, whether it be
 * one triggered by VBLANK, or a DMA routine finishing, etc. I love it. Kudos
 * to you Mr. Vijn. You're awesome, dude!
 * */

#define ADDR_OFS_DISPLAY_STAT   0x0004

#define ADDR_OFS_TIME_CNT0      0x0102
#define ADDR_OFS_TIME_CNT1      0x0106
#define ADDR_OFS_TIME_CNT2      0x010A
#define ADDR_OFS_TIME_CNT3      0x010E

#define ADDR_OFS_SIO_CNT        0x0128

#define ADDR_OFS_DMA_CNT0       0x00BA
#define ADDR_OFS_DMA_CNT1       0x00C6
#define ADDR_OFS_DMA_CNT2       0x00D2
#define ADDR_OFS_DMA_CNT3       0x00DE

#define ADDR_OFS_KEY_CNT        0x0132

#define IRQ_SRC_HNDL(regname, flag) \
  ((IRQ_Src_Handle_t){ADDR_OFS_##regname, flag})


typedef struct {
  u16_t src_reg_ofs;
  u16_t src_reg_flag;
} ALIGN(4) IRQ_Src_Handle_t;


#if USE_TONC_IRQ_TABLE_IMPL


#else
typedef IRQ_Callback_t IRQ_Entry_t;
#endif  /* GBA Cfg options (see include/gba_cfg.h for details). */

IRQ_Entry_t _internal_isr_table[IRQ_IDX_LIM + 1] = {0};

static const IRQ_Src_Handle_t _internal_irq_srcs[] = {
  IRQ_SRC_HNDL(DISPLAY_STAT,  0x0008),  // VBLANK
  IRQ_SRC_HNDL(DISPLAY_STAT,  0x0010),  // HBLANK
  IRQ_SRC_HNDL(DISPLAY_STAT,  0x0020),  // VCOUNT
  IRQ_SRC_HNDL(TIME_CNT0,     0x0040),  // Timer 0
  IRQ_SRC_HNDL(TIME_CNT1,     0x0040),  // Timer 1
  IRQ_SRC_HNDL(TIME_CNT2,     0x0040),  // Timer 2
  IRQ_SRC_HNDL(TIME_CNT3,     0x0040),  // Timer 3
  IRQ_SRC_HNDL(SIO_CNT,       0x4000),  // Serial IO
  IRQ_SRC_HNDL(DMA_CNT0,      0x4000),  // DMA Slot 0
  IRQ_SRC_HNDL(DMA_CNT1,      0x4000),  // DMA Slot 1
  IRQ_SRC_HNDL(DMA_CNT2,      0x4000),  // DMA Slot 2
  IRQ_SRC_HNDL(DMA_CNT3,      0x4000),  // DMA Slot 3
  IRQ_SRC_HNDL(KEY_CNT,       0x4000),  // Keypad
  {0,0}  // Null terminator type beat? Idk.
};

#define MEM_IO_UPTR 0x04000000ULL
#define DEREF_REG_ADDR(address) (*((vu16_t*) (address)))
#define DEREF_REG_OFS(offset) (*((vu16_t*) (MEM_IO_UPTR + offset)))

void IRQ_Enable(IRQ_Idx_t type) {
  const IRQ_Src_Handle_t* src;
  u16_t master_enable = REG_IME;

  REG_IME = 0;

  src = &_internal_irq_srcs[type];
  DEREF_REG_OFS(src->src_reg_ofs) |= src->src_reg_flag;
  REG_IE |= 1U<<type;
  REG_IME = master_enable;
}

void IRQ_Disable(IRQ_Idx_t type) {
	u16_t ime= REG_IME;
	REG_IME= 0;

	const IRQ_Src_Handle_t *src = &_internal_irq_srcs[type];
  DEREF_REG_OFS(src->src_reg_ofs) ^= src->src_reg_flag;
	REG_IE ^= 1<<type;
	REG_IME= ime;
}

void IRQ_Init(IRQ_Callback_t isr_main_callback) {
  REG_IME = 0;
  REG_ISR_MAIN = (isr_main_callback ? isr_main_callback : ISR_Master_ctl);
  REG_IME = 1;
}

IRQ_Callback_t IRQ_SetMaster(IRQ_Callback_t new_master_isr_cb) {
  IRQ_Callback_t ret = NULL;
  u16_t master_enable = REG_IME;
  REG_IME = 0;
  if (new_master_isr_cb) {
    ret = REG_ISR_MAIN;
    REG_ISR_MAIN = new_master_isr_cb;
  }
  REG_IME = master_enable;
  return ret;
}


IRQ_Callback_t IRQ_Add(IRQ_Idx_t type, IRQ_Callback_t cb) {
  u16_t master_enable = REG_IME;
  IRQ_Callback_t ret = NULL;
  REG_IME = 0;
  
  int i;
  u16_t flag;
  IRQ_Entry_t* ient = _internal_isr_table;

  const IRQ_Src_Handle_t* src = &_internal_irq_srcs[type];
  DEREF_REG_OFS(src->src_reg_ofs) |= src->src_reg_flag;
  REG_IE |= flag = 1U<<type;
  for (i=0; ient[i].flag; ++i) {
    if (ient[i].flag == flag)
      break;
  }

  ret = ient[i].isr_cb;

  ient[i].isr_cb = cb;
  ient[i].flag = flag;

  REG_IME = master_enable;
  return ret;
}


IRQ_Callback_t IRQ_Set(IRQ_Idx_t type, IRQ_Callback_t cb, int opts) {
  u16_t ime = REG_IME;
  REG_IME = 0;

  int i, slot, priority;
  u16_t  irq_flag = 1<<type;
  IRQ_Callback_t prev_cb = NULL;

  IRQ_Entry_t *ient = _internal_isr_table;
  const IRQ_Src_Handle_t *src = &_internal_irq_srcs[type];
  DEREF_REG_OFS(src->src_reg_ofs) |= src->src_reg_flag;
  REG_IE |= irq_flag;
  
  for (slot=0; ient[slot].flag; ++slot)
    if (ient[slot].flag == irq_flag)
      break;

  priority = opts&(INTERRUPT_OPTS_LAST|INTERRUPT_OPTS_PRIORITY_MASK);

  if (ient[slot].flag == irq_flag) {
    if ((opts&INTERRUPT_OPTS_REPLACE) || slot == priority) {
      prev_cb = ient[slot].isr_cb;
      ient[slot].isr_cb = cb;
      REG_IME = ime;
      return prev_cb;
    } else {
      do {
        ient[slot] = ient[slot+1];
      } while (ient[slot++].flag);
      --slot;
    }

  }

  
  if (priority < slot) {
    for (i = slot; i > priority; --i)
      ient[i] = ient[i-1];
    slot = i;
  }
  
  prev_cb = ient[slot].isr_cb;
  ient[slot].isr_cb = cb;
  ient[slot].flag = irq_flag;
  
  

  REG_IME = ime;
  return prev_cb;
}


IRQ_Callback_t IRQ_Rm(IRQ_Idx_t type) {
  u16_t ime = REG_IME;
  REG_IME = 0;

  int i;
  u16_t irq_flag = 1<<type;
  IRQ_Callback_t ret;
  IRQ_Entry_t *ient = _internal_isr_table;

  const IRQ_Src_Handle_t *src = &_internal_irq_srcs[type];
  DEREF_REG_OFS(src->src_reg_ofs) ^= src->src_reg_flag;

  REG_IE ^= irq_flag;

  for (i=0; ient[i].flag; ++i)
    if (ient[i].flag == irq_flag)
      break;

  ret = ient[i].isr_cb;

  for (; ient[i].flag; ++i)
    ient[i] = ient[i+1];

  REG_IME = ime;
  return ret;

}

