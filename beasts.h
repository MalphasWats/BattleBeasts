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

#endif