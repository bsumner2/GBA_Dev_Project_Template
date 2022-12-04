#include "gba.h"

int main()
{
    _init_Registers();
    _sound4_playSound(1);
    while(1)
    {
        vsync();
        if(_keyPressed(A))
            _sound4_playSound(1);
        
    }

}
