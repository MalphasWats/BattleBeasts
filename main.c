#include "main.h"
#include "beasts.h"

int main (void) 
{    
    initialise();
    
    uint32_t t = 0;

    uint8_t buttons;
    
    uint8_t debounce = 0;
    bool button_was_down = FALSE;
    
    for(ever)
    {
        t = millis();
        
        for (uint16_t i=0 ; i<(SCREEN_WIDTH*SCREEN_ROWS) ; i++)
            buffer[i] = TITLE[i];
        
        draw();
        
        buttons = read_buttons();
        if(buttons == BTN_A && !button_was_down)
        {
            button_was_down = TRUE;
            debounce = t+10;
        }
        
        if (buttons != BTN_A && button_was_down && debounce <= t)
        {
            button_was_down = FALSE;
            battle_beasts();
        }
    }
}

