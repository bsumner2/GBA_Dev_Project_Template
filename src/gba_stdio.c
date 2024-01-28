#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gba_bitfield_macros.h"
#include "reduced_ascii.h"
#include "gba_def.h"
#include "gba_functions.h"
#include "gba_inlines.h"
#include "gba_types.h"
// 1bpp 8x8 pixel buffer represents glyph
typedef u8_t Glyph_t[8];
//typedef enum { } SymType;

static const Glyph_t *ascii_font = ((Glyph_t*)reduced_ascii_glyphset);

static const char ESC_FLAG_CHAR = '\x1b';
 
static int ipow(int base, u16_t power) {
  if (power&1) return base*ipow(base*base, power>>1);
  else return power ? ipow(base*base, power>>1) : 1;
}

#define PARSE_ESC_CLR_ERROR_FLAG 0x8000

/*
 * @brief Use after encountering escape char, ASCII(27). Advance the pointer to
 * so that it's pointing to the char immediately following the esc char.
 * */
static u16_t parse_color(char **buf_ptr) {
  char *str = *buf_ptr;
  u16_t ret = 0;
  char tmp;
  int len = 0, i = 0;
  if (*str++ != '[')
    return PARSE_ESC_CLR_ERROR_FLAG;
  if (strncmp(str++, "0x", 2))
    return PARSE_ESC_CLR_ERROR_FLAG;
  while ((tmp = *++str) && tmp != ']') {
    if (!isxdigit(tmp) || ++len > 4)
      return PARSE_ESC_CLR_ERROR_FLAG;
  }
  if (!tmp || !len)
    return PARSE_ESC_CLR_ERROR_FLAG;
  if (!str[1]) {
    *buf_ptr = ++str;
    return 0;  // Color won't matter anyway if EOS reached.
  }
  for (tmp = tolower(*(str - 1 - (i = 0))); i < len;
      tmp = tolower(*(str - 1 - ++i))) {
    tmp = (tmp > '9' ? 10 + tmp - 'a' : tmp - '0');
    ret += tmp*ipow(16, i);
  }
  *buf_ptr = ++str;
  return 0x7FFF&ret;
}


bool_t Mode3_vprintf(u32_t x, u32_t y, u16_t color, const char *restrict fmt,
    va_list args) {
  if (!fmt)
    return false;
  if (y+8 > SCREEN_HEIGHT)
    return false;

  size_t len;
  u16_t *vbuf_row;
  u8_t *cur_glyph;
  char *cursor;
  const int startx = x;
  u16_t col = color;
  char tmp;
  u8_t glyphrow;

  {
    va_list args_copy;
    va_copy(args_copy, args);
    len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);
  }

  char buf[len+1];
  vsnprintf(buf, len + 1, fmt, args);
  if (buf[len]) 
    buf[len] = '\0';

  cursor = buf;
  while ((tmp = *cursor++)) {
    if (tmp < reduced_ascii_ASCII_OFS) {
      if (tmp=='\n') {
        y+=8;
        x = startx;
        if (y+8 > SCREEN_HEIGHT)
          return false;
      } else if (tmp == ESC_FLAG_CHAR) {
        if ((col = parse_color(&cursor)) & PARSE_ESC_CLR_ERROR_FLAG)
          return false;
      }
      continue;
    }

    if (x+8 > SCREEN_WIDTH) {
      y+=8;
      x = startx;
      if (y+8 > SCREEN_HEIGHT)
        return false;
    }

    cur_glyph = (u8_t*)ascii_font[tmp - reduced_ascii_ASCII_OFS];
    for (int i = 0; i < 8; ++i) {
      glyphrow = cur_glyph[i];
      vbuf_row = (y+i)*SCREEN_WIDTH + x + VIDEO_BUF;
      for (int j = 0; j < 8; ++j) {
        if (!(glyphrow&(0x80>>j)))
          continue;
        else
          vbuf_row[j] = col;
      }
    }
    x += 8;
  }

  return true;
  
   
}



/** 
 * @brief Print formatted string, fmt, with topleft starting point, (x,y) using
 * starting color, color.
 * @details Color flag should be formatted as ^ESC[<<color hexcode>>]. ^ESC
 * is ASCII(27), and can be represented as <<backslash>>x1b or <<backslash>>033
 * */
bool_t Mode3_printf(u32_t x, u32_t y, u16_t color, const char *restrict fmt, 
    ...) {
  if (!fmt)
    return false;
  
  if (y + 8 > SCREEN_HEIGHT)
    return false;

  va_list args;
  size_t len;
  u16_t *vbuf_row;
  u8_t *cur_glyph;
  char *cursor;
  const int startx = x;
  u16_t col = color;
  char tmp;
  u8_t glyphrow;


  va_start(args, fmt);
  len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  // Now that we have the length of what we need to print...
  char buf[len+1];
  va_start(args, fmt);
  vsnprintf(buf, len+1, fmt, args);
  if (buf[len]) buf[len] = '\0';
  va_end(args);

  cursor = buf;
  while ((tmp = *cursor++)) {
    if (tmp < reduced_ascii_ASCII_OFS) {
      if (tmp=='\n') {
        y+=8;
        x = startx;
        if (y+8 > SCREEN_HEIGHT)
          return false;
      } else if (tmp == ESC_FLAG_CHAR) {
        if ((col = parse_color(&cursor)) & PARSE_ESC_CLR_ERROR_FLAG)
          return false;
      }
      continue;
    }

    if (x+8 > SCREEN_WIDTH) {
      y+=8;
      x = startx;
      if (y+8 > SCREEN_HEIGHT)
        return false;
    }

    cur_glyph = (u8_t*)ascii_font[tmp - reduced_ascii_ASCII_OFS];
    for (int i = 0; i < 8; ++i) {
      glyphrow = cur_glyph[i];
      vbuf_row = (y+i)*SCREEN_WIDTH + x + VIDEO_BUF;
      for (int j = 0; j < 8; ++j) {
        if (!(glyphrow&(0x80>>j)))
          continue;
        else
          vbuf_row[j] = col;
      }
    }
    x += 8;
  }

  return true;
}







/** 
 * @brief Put string starting from topleft offset x, y with the color provided.
 * @return True if able to write everything, False if truncation was required.
 * */
bool_t Mode3_puts(const char* restrict s, int x, int y, u16_t color) {
  const char *cur = s;
  u16_t *vbuf_row;
  u8_t *curr_glyph;
  const int startx=x;
  char c;
  u8_t tmp;
  if (y + 8 > SCREEN_HEIGHT)
    return false;
  while ((c = *cur++)) {
    if (c < reduced_ascii_ASCII_OFS) {
      if (c=='\n') {
        x = startx;
        y+=8;
      }
      continue;
    }
    if (x+8 > SCREEN_WIDTH) {
      y += 8, x = startx;
      if (y + 8 > SCREEN_HEIGHT)
        return false;
    }
    curr_glyph = (u8_t*)ascii_font[c - reduced_ascii_ASCII_OFS];
    for (int i = 0; i < 8; ++i) {
      tmp = curr_glyph[i];
      vbuf_row = (y+i)*SCREEN_WIDTH + x + VIDEO_BUF;
      for (int j = 0; j < 8; ++j) {
        if (!(tmp&(0x80>>j)))
          continue;
        else
          vbuf_row[j] = color;
      }
    }
    x += 8;
  }

  return true;
}
bool_t Mode3_putchar(u32_t x, u32_t y, char c, u16_t color) {
  if (c <= reduced_ascii_ASCII_OFS || c==127)
    return false;
  if (y+8>SCREEN_HEIGHT || x+8>SCREEN_WIDTH)
    return false;
  u16_t *vbuf_row;
  u8_t *glyph = (u8_t*)ascii_font[c - reduced_ascii_ASCII_OFS], tmp;
  for(int i = 0; i < 8; ++i) {
    vbuf_row = &VIDEO_BUF[x + SCREEN_WIDTH*(i+y)];
    tmp = glyph[i];
    for (int j = 0; j < 8; ++j) {
      if (!(tmp&(0x80>>j)))
        continue;
      else
        vbuf_row[j] = color;
    }
  }
  return true;
}



#define Mode3_putc(...) Mode3_putchar(__VA_ARGS__)

#if 0

typedef struct {
  char keys[64];
  u8_t *key_group_ofss;
  u32_t key_group_count;
  u16_t cursor, key_ct, fg, sel_fg, bg, sel_bg;
} Mode3_KeyPad;


Mode3_KeyPad *KeyPad_Init(Mode3_KeyPad *dst, u16_t fg, u16_t sel_fg, u16_t bg, 
    u16_t sel_bg, const char symbol_ranges[][2], int symbol_ranges_len) {
  int last = 0, lb, hb;
  dst->key_group_count = symbol_ranges_len;
  dst->key_group_ofss = (u8_t*)malloc(sizeof(u8_t)*symbol_ranges_len);
  for(int i = 0; i < symbol_ranges_len; ++i) {
    lb = symbol_ranges[i][0], hb = symbol_ranges[i][1];
    if ((lb|hb++)&0x80) {  // No negative values allowed!
      free((void*)(dst->key_group_ofss));
      return NULL;
    }
    dst->key_group_ofss[i] = last;
    for (char j = lb; j < hb; ++j) {
      if (last > 61) {  // last two idxs are reserved for backspace and enter
        free((void*)(dst->key_group_ofss));
        return NULL;
      }

      dst->keys[last++] = j;
    }
  }
  {
    u16_t clrs[4] = {
      fg, sel_fg, bg, sel_bg
    };
    *((u32_t*) (&(dst->fg))) = *((u32_t*) (&clrs[0]));
    *((u32_t*) (&(dst->bg))) = *((u32_t*) (&clrs[2]));
  }
  dst->key_ct = last;
  dst->cursor = 0;
  return dst;
}

void KeyPad_Close(Mode3_KeyPad *kp) {
  free((void*)(kp->key_group_ofss));
  memset(kp, 0, sizeof(Mode3_KeyPad));
}


void Mode3_FillRect(u16_t x, u16_t y, u16_t w, u16_t h, u16_t color) {
  if (y+h > SCREEN_HEIGHT || x+w > SCREEN_WIDTH)
    return;
  u16_t *row = (VIDEO_BUF + y*SCREEN_WIDTH + x), row_hop_len = SCREEN_WIDTH-w,
        cur;

  while (h--) {
    cur = w;
    while (cur--)
      *row++ = color;
    row += row_hop_len;
  }
}


char *Mode3_KPgets(char *buf, size_t maxlen, Mode3_KeyPad *kp) {
  if (!kp || !buf || !maxlen)
    return NULL;
  int cur_rowlen = 0, cur_collen, cur_objct;
  for (u32_t i = 0; i < kp->key_group_count; ++i) {
    
    
    
  }
  

  return NULL;  // TODO remove stub placeholder
}




int main(void) {
  REG_DISPLAY_CNT_SET_MODES(DCNT_V_MODE3, DCNT_BG_MODE2);
  u16_t col = 0, x = 0, y = 8;
  for (int i = 0; i < reduced_ascii_glyph_count; ++i) {
    if (y + 8 > SCREEN_HEIGHT) {
      y = 8;
      x+=8;
    }
    Mode3_putchar(x, y, reduced_ascii_ASCII_OFS+i, 0x7FFF);
    y+=8;
  }
  x = y = 0;
  while (1) {

    vsync();
    if (x+9>SCREEN_WIDTH) {
      x = 0;
      y += 9;
      if (y+9>SCREEN_HEIGHT)
        break;
    }

    Mode3_printf(0, 0, 0,"\x1b[0x%X]Hello World!\n", col); 
    x += 9;
  }

  while (1);
 
}
#endif


