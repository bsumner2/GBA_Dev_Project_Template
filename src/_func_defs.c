#include "gba.h"



void _init_Registers()
{
    //~~~~~~~~~~~~~~~~~~~~~~Set draw mode in display register to VIDEO_MODE | BG_MODE = 0000 0000 0000 0011 | 0000 0100 0000 0000 = 0000 0100 0000 0011
    REG_DISPLAY = VIDEO_MODE | BG_MODE;

    //~~~~~~~~~~~~~~~~~~~~~~Set up the sound register to enable sound
    //Enable the master volume by flipping sound stat register bit number 7 to a 1, dont care about other registers so we can just overwrite with MASTER_SOUND_ENABLE( = 0x0080)
    REG_SOUND_STAT = MASTER_SOUND_ENABLE;
    
    //Set master volume to 100% volume
    REG_SOUND_DS_COUNT = DMG_VOL_100;

    /* UNCOMMENT FOR SOUND1
    REG_SOUND_DMG_COUNT = SOUND_DMG_BUILD_LR(SOUND_DMG_SQUARE_1, 7);
   //Turn off sweeping
    REG_SOUND1_SWEEP = SOUND1_SWEEP_OFF;
    //Envelope = Vol 12/15ths, Decaying, Max Step-Time 7, Duty 50%
    REG_SOUND1_COUNT = SOUND_SQUARE_ENVELOPE_BUILD(12, 0, 7) | SOUND_SQUARE_DUTY1_2ND;

    REG_SOUND1_FREQUENCY = 0;*/

    /* SOUND4 */
    REG_SOUND_DMG_COUNT = SOUND_DMG_BUILD_LR(SOUND_DMG_NOISE, 7);

    //Envelope = Vol 12/15ths, Decaying, Max Step-Time 7, Duty 50%
    REG_SOUND4_COUNT = SOUND_SQUARE_ENVELOPE_BUILD(12, 0, 7) | SOUND_SQUARE_DUTY1_2ND;
    REG_SOUND4_FREQUENCY = 0;


}





/**
 * @brief Need to fix frame stutter with a syncing method. 
 * 
 */
void vsync()
{
    while(REG_DISPLAY_VCOUNT>=160); //Wait until out of V-Blank, if in V-Blank.
    while(REG_DISPLAY_VCOUNT<160);  //Wait for last fame to finish drawing before setting screen buffer values for current frame.
}
