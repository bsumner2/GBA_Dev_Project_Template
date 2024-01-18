#ifndef _REDUCED_ASCII_H_
#define _REDUCED_ASCII_H_

#ifdef __cplusplus
extern "C" {
#endif  /* C++ Name Mangler Guard */

extern const unsigned short reduced_ascii_glyphset[];
#define reduced_ascii_glyph_count 96
#define reduced_ascii_ASCII_OFS 32

#define GLYPH_DIMS 8
#define GLYPH_BPP 1

// (8x8 pixels)*(1 bits/pixel)/(8 bits)/Byte = 8x8/8 = 8 Bytes/Glyph
#define GLYPH_BYTE_LEN 8

#ifdef __cplusplus
}
#endif  /* C++ Name Mangler Guard */

#endif  /* _REDUCED_ASCII_H_*/

