/** Written by Burton O Sumner With guidance from USC Columbia's Dr. William Hoskins and the Tonc GBA-Dev Libraries (https://www.coranac.com/tonc/text/toc.htm) */
/** Function declarations */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H




void vsync();

void _init_Registers();

void main_Menu();

void game_Loop();

static inline void _set_sound_mode(uint8 channel)
                {    REG_SOUND_DMG_COUNT = SOUND_DMG_BUILD_LR(channel, 7);  }

static inline uint16 _keyPressed(uint16 key)
                    {   return !(REG_DISPLAY_INPUT&key);    }


static inline void _sound1_playSound(int note, int octave, int length)
{
    
    REG_SOUND1_COUNT &= 0xFFC0; //Overwrite previously-played note's length with zeros.
    REG_SOUND1_COUNT |= ((length)&63);
    REG_SOUND1_FREQUENCY = SOUND_FREQUENCY_RESET | SOUND_RATE(note, octave);
    REG_SOUND1_FREQUENCY |= (length!=0) ? SOUND_TIMED : 0; //Else or by 0 because 1 OR 0 is 1, so basically nothing changes.
}




static inline uint8 _valid_noise_prestep_freq(uint8 freq)
{   return (freq>=0b1110) ? 0b1101 : freq;    }

static inline void _sound4_playSound(int length)
{
    REG_SOUND4_COUNT &= 0xFFC0;
    REG_SOUND4_COUNT |= ((length)&63);
    REG_SOUND4_FREQUENCY = 0;
    REG_SOUND4_FREQUENCY = SOUND_FREQUENCY_RESET | SOUND_NOISE_FRQ_BUILD(1, _valid_noise_prestep_freq(13), 1, 7);
}



static inline void mode3_Plot(uint32 x, uint32 y, uint16 color)
{
    SCREENBUFFER[x + SCREEN_WIDTH*y] = color;
}




#endif /* FUNCTIONS_H */


