#include "beasts.h"

void battle_beasts(void)
{
    uint32_t t = 0;
	
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
	
	generate_beast(player_beast);
    
    for(ever)
    {
        t = millis();
        
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
				if (beast[x] & (1 << (6-y)))
					beast[x] |= 1 << y;
		}
	}
}

void draw_beast(uint8_t* beast)
{
	for(uint8_t i=0 ; i<8 ; i++)
		buffer[ (6*SCREEN_WIDTH+(2*8)) + i ] = beast[i];
}