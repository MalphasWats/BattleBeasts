#include "beasts.h"

void battle_beasts(void)
{
    uint32_t t = 0;
    uint32_t gen_timer = 0;
    
    uint8_t player_beast[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    
    for(ever)
    {
        t = millis();
        
        if (gen_timer <= t)
        {
            gen_timer = t+2000;
            
            for(uint8_t i=0 ; i<8 ; i++)
                player_beast[i] = 0x00;
            
            generate_beast(player_beast);
        }
        
        clear_buffer();
        
        draw_beast(player_beast);
            
        draw();
    }
}

void generate_beast(uint8_t* beast)
{
    for(uint8_t y=0 ; y<7 ; y++)
    {
        for (uint8_t x=0 ; x<7 ; x++)
        {
            if (x < 4)
                beast[x] |= (rng() & 1) << y;
            else
                if (beast[6-x] & (1 << y))
                    beast[x] |= 1 << y;
        }
    }
}

void draw_beast(uint8_t* beast)
{
    // Resize beast (x2, 16x16)
    //TODO: this can only double 8x8
    uint8_t resize_buff[32];
    
    uint8_t x2, y2;
    uint16_t temp;
    for (uint8_t i=0 ; i<16 ; i++) 
    {
        temp = 0;
        for (uint8_t j=0 ; j<16 ; j++) 
        {
            x2 = (uint8_t)((i*32769)>>16) ;
            y2 = (uint8_t)((j*32769)>>16) ;
            if (beast[x2] & (1 << y2))
                temp |= 1 << j;
        }
        //re-assemble temp into resize_buff
        resize_buff[i] = temp & 0x00ff;
        resize_buff[i+16] = temp>>8;
    }
    
    for(uint8_t i=0 ; i<16 ; i++)
    {
        buffer[ (5*SCREEN_WIDTH+(2*8)) + i ] = resize_buff[i];
        buffer[ (6*SCREEN_WIDTH+(2*8)) + i ] = resize_buff[i+16];
    }
}