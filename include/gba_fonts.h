#ifndef _GBA_FONTS_H_
#define _GBA_FONTS_H_

#include "gba_types.h"

typedef struct s_fctx {
  const void *glyph_data;
  u16_t glyph_ct;
  const u8_t *widths;
  const u8_t *heights;
  u8_t glyph_width;
  u8_t glyph_height;
  u8_t cell_width;
  u8_t cell_height;
  u16_t cell_len;
  u8_t bitdepth;
  u8_t padding;
#ifdef __cplusplus
  s_fctx(const void *glyph_table, u16_t glyph_ct, const u8_t *glyph_width_table, const u8_t *glyph_height_table, u8_t glyph_width, u8_t glyph_height, u8_t cell_width, u8_t cell_height, u16_t cell_len, u8_t bitdepth=1) {
    this->glyph_data = glyph_table;
    this->glyph_ct = glyph_ct;
    this->widths = glyph_width_table;
    this->heights = glyph_height_table;
    this->glyph_width = glyph_width;
    this->glyph_height = glyph_height;
    this->cell_width = cell_width;
    this->cell_height = cell_height;
    this->cell_len = cell_len;
    this->bitdepth = bitdepth;
  }
#endif
} Font_Ctx_t;




typedef enum {
  BMP=1,
  BMP8=3,
  BMP16=5,

} TextRenderType_t;


typedef struct s_txtrendersurface {
  void *surface_vram;
  u32_t pitch;
  u16_t width;
  u16_t height;
  u8_t bpp;
  u8_t surface_render_type;


#ifdef __cplusplus
#endif
} TextRenderCtx_t;

#endif  /* _GBA_FONTS_H_ */
