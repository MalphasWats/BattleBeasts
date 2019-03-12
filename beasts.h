#ifndef BEASTS_H
#define BEASTS_H

#include "SPIKE.h"
#include "tilemap-engine.h"


static const __flash uint8_t TILES[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
};

static const __flash uint8_t MASKS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // SPACE
};

void battle_beasts(void);

void generate_beast(uint8_t* beast);

//void draw_beast(uint8_t* beast);
void scale_beast(uint8_t* beast, uint8_t* return_buffer, uint8_t scale_factor);
void draw_beast(uint8_t* beast, uint8_t x, uint8_t y, uint8_t w);

#endif