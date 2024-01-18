#ifndef _GBA_STDIO_H_
#define _GBA_STDIO_H_

#include "gba_types.h"
#include "gba_util_macros.h"

STAT_INLN void 



bool_t Mode3_printf(u32_t x, u32_t y, const char *restrict fmt, 
    const u16_t *restrict clrs, ...);

bool_t Mode3_puts(const char* restrict s, int x, int y, u16_t color);

bool_t Mode3_putchar(u32_t x, u32_t y, char c, u16_t color);



#endif  /* _GBA_STDIO_H_ */
