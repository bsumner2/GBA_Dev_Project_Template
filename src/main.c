/** Written by Burton O Sumner With guidance from USC Columbia's Dr. William Hoskins and the Tonc GBA-Dev Libraries (https://www.coranac.com/tonc/text/toc.htm) */
#include "gba.h"

int main()
{
    _init_Registers();
    _sound4_playSound(1);
    while(1)
    {
        // Just plays a white noise sound if "A" button is pressed. Was just the
        // last thing I tested before committing this to github.
        vsync();
        if(_keyPressed(A))
            _sound4_playSound(1);
        
    }

}
