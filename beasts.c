#include "beasts.h"

Beast player;

void new_game()
{
    generate_beast(player.beast);
    battle_beasts();
}

void battle_beasts(void)
{
    uint32_t t = 0;

    uint8_t beast_buffer[32*4];
    for(uint8_t i=0 ; i<32*4 ; i++)
        beast_buffer[i] = 0x00;

    scale_beast(player.beast, beast_buffer, 4);

    for(ever)
    {
        t = millis();

        clear_buffer();

        draw_beast(beast_buffer, 32, 32, 32);

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

// http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
void scale_tile(__memx uint8_t* tile, uint8_t* return_buffer, uint8_t scale_factor)
{
    //TODO: if scale factor is 1, this breaks, but why are you trying to scale something by 1?
    uint8_t w2 = 8 * scale_factor;
    uint8_t ratio = ((8<<8) / w2) +1;

    uint8_t x2, y2;

    for (uint8_t x=0 ; x<w2 ; x++)
    {
        for (uint8_t y=0 ; y<w2 ; y++)
        {
            x2 = (uint8_t)((x*ratio)>>8);
            y2 = (uint8_t)((y*ratio)>>8);
            if (tile[x2] & (1 << y2))
                return_buffer[ (y>>3) * w2 + x ] |= 1 << (y&7);
        }
    }
}

void draw_beast(uint8_t* beast, uint8_t x, uint8_t y, uint8_t w)
{
    y = y>>3;
    for(uint8_t img_y=0 ; img_y<(w>>3) ; img_y++)
        for(uint8_t img_x=0 ; img_x<w ; img_x++)
            buffer[(img_y+y)*SCREEN_WIDTH + (img_x+x)] = beast[img_y*w + img_x];
}
