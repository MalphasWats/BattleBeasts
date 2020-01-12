#include "samd21g18au.h"

#define COL PORT_PB08
#define ROW PORT_PA15

/* These are missing from the io definitions, I don't know why. */
#define PORT_PB10                 (1ul << 10)
#define PORT_PB11                 (1ul << 11)
#define PORT_PB22                 (1ul << 22)
#define PORT_PB23                 (1ul << 23)

#define ALL_COLS PORT_PB03 | PORT_PB02 | PORT_PB09 | PORT_PB08 | PORT_PB22 | PORT_PB23 | PORT_PB11 | PORT_PB10
#define ALL_ROWS PORT_PA15 | PORT_PA14 | PORT_PA13 | PORT_PA12 | PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA11

#define TIMER_INTERVAL 100

static const uint32_t COLS[] = {
    PORT_PB03,
    PORT_PB02,
    PORT_PB09,
    PORT_PB08,
    PORT_PB22,
    PORT_PB23,
    PORT_PB11,
    PORT_PB10,
};

static const uint32_t ROWS[] = {
    PORT_PA15,
    PORT_PA14,
    PORT_PA13,
    PORT_PA12,
    PORT_PA08,
    PORT_PA09,
    PORT_PA10,
    PORT_PA11,
};

void simple_delay(uint16_t);
uint8_t rng( void );
void generate_beast(uint8_t* beast);


void simple_delay(uint16_t n)
{
    for (;n >0; n--)
    {
        for (uint8_t i=0;i<100;i++)
            __asm("nop");
    }
}


#define rngM 251
#define rngA 11
#define rngC 3
uint8_t rngSEED = 5;
uint8_t rng( void )
{
    rngSEED = (rngSEED*rngA +rngC) % rngM;
    return rngSEED;
}

void generate_beast(uint8_t* beast)
{
    for(uint8_t y=0 ; y<8 ; y++)
    {
        for (uint8_t x=0 ; x<8 ; x++)
        {
            if (x < 4)
                beast[x] |= (rng() & 1) << y;
            else if (x == 4) {
                beast[x] = beast[x-1];
            }
            else
                if (beast[7-x] & (1 << y))
                    beast[x] |= 1 << y;
        }
    }
}

int main()
{
    /* Set LED Matrix pins to output */
    REG_PORT_DIRSET0 = ALL_ROWS;
    REG_PORT_DIRSET1 = ALL_COLS;

    /* Set LED Matrix to 'Off' */
    REG_PORT_OUTCLR1 = ALL_COLS;
    REG_PORT_OUTSET0 = ALL_ROWS;

    uint32_t col = 0;
    uint8_t beast[8];

    uint16_t timer = TIMER_INTERVAL;

    while(1)
    {
        timer -= 1;
        if (timer == 0){
            timer = TIMER_INTERVAL;
            for(uint8_t i=0 ; i<8 ; i++)
                beast[i] = 0;
            generate_beast(beast);
        }

        for (uint8_t r=0 ; r<8 ; r++) {
            REG_PORT_OUTCLR1 = ALL_COLS;
            REG_PORT_OUTSET0 = ALL_ROWS;
            REG_PORT_OUTSET1 = COLS[r];

            col = 0;
            for(uint8_t c=0 ; c<8 ; c++) {

                if (beast[r] & (1<<c))
                    col |= ROWS[c];
            }
            REG_PORT_OUTCLR0 = col;

            /* Without a delay the Matrix LEDs are dim because they
               only spend 1/8th of the total time ON */
            simple_delay(1);
        }
    }
}
