//GBA Project 2 GBA Header file.
/* main.c
 * Written by Burton O Sumner
 * With Partial In-class, Lecture-Guidance from Dr. Hoskins in CSCE212
 * For Homework Assignment 3 For CSCE212 taught by Dr. Hoskins
 */
//240x160
//ARM7TDMI 16.78 MHz
//Color word is 16bits
//0bXbbbbbgggggrrrrr
//0x00000000 -> 0x00003fff (System ROM, exe but not readable)
//0x02000000 -> 0x02020000 (External RAM, 256 KB)
//0x03000000 -> 0x03007fff (Internal RAM, 32 KB)
//0x04000000 -> 0x04003fff (IO registers)
//0x05000000 -> 0x05003fff (Color Palette)
//0x06000000 -> 0x06017fff (VRAM 96 KB)
//0x07000000 -> 0x070003ff (OAM RAM)
//0x08000000 -> 0x???????? (Gamepak ROM up to 32MB)
//0x0E000000 -> 0x???????? (Gamepak RAM)


#ifndef GBA_H
#define GBA_H
#include "types.h"
#include "registerdefs.h"
#include "functions.h"




#define COLOR_BLACK                     0x0000
#define COLOR_MENU_BG                   0x1460
#define COLOR_MENU_OPTION               0x2480
#define COLOR_MENU_OPTION_HILIGHTED     0x7A42


#endif // GBA_H
