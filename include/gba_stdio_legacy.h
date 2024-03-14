#ifndef _GBA_STDIO_LEGACY_H_
#define _GBA_STDIO_LEGACY_H_


#ifdef __cplusplus
#define restrict
extern "C" {
#endif  /* CXX Header Guard */


#include <stdarg.h>
#include "gba_types.h"
#include "gba_util_macros.h"



/**
 * @brief Takes an initialized variadic args list. Args will not be closed after
 * use. It is the caller's responsibility to call va_end the va_list passed 
 * to the function.
 * */
bool_t Mode3_vprintf(u32_t x, u32_t y, u16_t color, const char *restrict fmt,
    va_list args);

/** 
 * @brief Print formatted string, fmt, with topleft starting point, (x,y) using
 * starting color, color.
 * @details Color flag should be formatted as ^ESC[<<color hexcode>>]. ^ESC
 * is ASCII(27), and can be represented as <<backslash>>x1b or <<backslash>>033
 * */
bool_t Mode3_printf(u32_t x, u32_t y, u16_t color, const char *restrict fmt, 
    ...);

bool_t Mode3_puts(const char* restrict s, int x, int y, u16_t color);

bool_t Mode3_putchar(u32_t x, u32_t y, char c, u16_t color);


#ifdef __cplusplus
}
#undef resctrict
#endif  /* CXX Header Guard */

#endif  /* _GBA_STDIO_LEGACY_H_ */
