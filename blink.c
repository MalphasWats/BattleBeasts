#include "samd21g18au.h"

#define COL PORT_PB08
#define ROW PORT_PA15

//#define ROWS    REG_PORT_OUT1
//#define COLUMNS REG_PORT_OUT0
#define PORT_PB10                 (1ul << 10)
#define PORT_PB11                 (1ul << 11)
#define PORT_PB22                 (1ul << 22)
#define PORT_PB23                 (1ul << 23)

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

#define ALL_COLS PORT_PB03 | PORT_PB02 | PORT_PB09 | PORT_PB08 | PORT_PB22 | PORT_PB23 | PORT_PB11 | PORT_PB10
#define ALL_ROWS PORT_PA15 | PORT_PA14 | PORT_PA13 | PORT_PA12 | PORT_PA08 | PORT_PA09 | PORT_PA10 | PORT_PA11

static const uint8_t IMG[] = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b01011010,
    0b00100100,
    0b00011000,

};

void delay(int n)
{
    int i;

    for (;n >0; n--)
    {
        for (i=0;i<100;i++)
            __asm("nop");
    }
}

void set_clock()
{
    // Change the timing of the NVM access
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val; // 1 wait state for operating at 2.7-3.3V at 48MHz.

    // Enable the bus clock for the clock system.
    PM->APBAMASK.bit.GCLK_ = 1; //true;

    // Initialise the DFLL to run in closed-loop mode at 48MHz
    // 1. Make a software reset of the clock system.
    GCLK->CTRL.bit.SWRST = 1; //true;
    while (GCLK->CTRL.bit.SWRST && GCLK->STATUS.bit.SYNCBUSY) {};
    // 2. Make sure the OCM8M keeps running.
    SYSCTRL->OSC8M.bit.ONDEMAND = 0;
    // 3. Set the division factor to 64, which reduces the 1MHz source to 15.625kHz
    GCLK->GENDIV.reg =
        GCLK_GENDIV_ID(3) | // Select generator 3
        GCLK_GENDIV_DIV(64); // Set the division factor to 64
    // 4. Create generic clock generator 3 for the 15KHz signal of the DFLL
    GCLK->GENCTRL.reg =
        GCLK_GENCTRL_ID(3) | // Select generator 3
        GCLK_GENCTRL_SRC_OSC8M | // Select source OSC8M
        GCLK_GENCTRL_GENEN; // Enable this generic clock generator
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
    // 5. Configure DFLL with the
    GCLK->CLKCTRL.reg =
        GCLK_CLKCTRL_ID_DFLL48 | // Target is DFLL48M
        GCLK_CLKCTRL_GEN(3) | // Select generator 3 as source.
        GCLK_CLKCTRL_CLKEN; // Enable the DFLL48M
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
    // 6. Workaround to be able to configure the DFLL.
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;  //false
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 7. Change the multiplication factor.
    SYSCTRL->DFLLMUL.bit.MUL = 3072; // 48MHz / (1MHz / 64)
    SYSCTRL->DFLLMUL.bit.CSTEP = 1; // Coarse step = 1
    SYSCTRL->DFLLMUL.bit.FSTEP = 1; // Fine step = 1
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 8. Start closed-loop mode
    SYSCTRL->DFLLCTRL.reg |=
        SYSCTRL_DFLLCTRL_MODE | // 1 = Closed loop mode.
        SYSCTRL_DFLLCTRL_QLDIS; // 1 = Disable quick lock.
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 9. Clear the lock flags.
    //SYSCTRL->INTFLAG.bit.DFLLLCKC = 1;
    //SYSCTRL->INTFLAG.bit.DFLLLCKF = 1;
    //SYSCTRL->INTFLAG.bit.DFLLRDY = 1;
    // 10. Enable the DFLL
    SYSCTRL->DFLLCTRL.bit.ENABLE = 1;//true;
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY) {}; // Wait for synchronization.
    // 11. Wait for the fine and coarse locks.
    while (!SYSCTRL->INTFLAG.bit.DFLLLCKC && !SYSCTRL->INTFLAG.bit.DFLLLCKF) {};
    // 12. Wait until the DFLL is ready.
    while (!SYSCTRL->INTFLAG.bit.DFLLRDY) {};

    // Switch the main clock speed.
    // 1. Set the divisor of generic clock 0 to 0
    GCLK->GENDIV.reg =
        GCLK_GENDIV_ID(0) | // Select generator 0
        GCLK_GENDIV_DIV(0);
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
    // 2. Switch generic clock 0 to the DFLL
    GCLK->GENCTRL.reg =
        GCLK_GENCTRL_ID(0) | // Select generator 0
        GCLK_GENCTRL_SRC_DFLL48M | // Select source DFLL
        GCLK_GENCTRL_IDC | // Set improved duty cycle 50/50
        GCLK_GENCTRL_GENEN; // Enable this generic clock generator
    while (GCLK->STATUS.bit.SYNCBUSY) {}; // Wait for synchronization
}

int main()
{
    REG_PORT_DIRSET0 = ALL_ROWS;
    REG_PORT_DIRSET1 = ALL_COLS;

    //COLUMNS &= ~COL0;
    //COLUMNS |= COL0;

    REG_PORT_OUTCLR1 = ALL_COLS;
    REG_PORT_OUTSET0 = ALL_ROWS;

    //REG_PORT_OUTSET1 = COLS[3] | COLS[2] | COLS[1] | COLS[0] ;
    //REG_PORT_OUTCLR0 = ROWS[0];

    //REG_PORT_OUTSET0 = ROW;
    //REG_PORT_OUTCLR1 = COL;

    /*delay(1000);

    REG_PORT_OUTSET1 = COLS_OFF;
    REG_PORT_OUTCLR0 = ROWS_OFF;*/

    //uint8_t c = 1;
    //uint8_t r = 0;

    //REG_PORT_OUTCLR0 = ALL_ROWS;
    //REG_PORT_OUTSET1 = COLS[0];

    //set_clock();

    //REG_PORT_OUTSET1 = COLS_OFF;*/

    //REG_PORT_OUTSET1 = ALL_COLS;

    uint32_t col = 0;

    while(1)
    {
        for (uint8_t r=0 ; r<8 ; r++) {
            REG_PORT_OUTCLR1 = ALL_COLS;
            REG_PORT_OUTSET0 = ALL_ROWS;
            REG_PORT_OUTSET1 = COLS[r];

            col = 0;
            for(uint8_t c=0 ; c<8 ; c++) {

                if (IMG[c] & (1<<r))
                    col |= ROWS[c];

                //REG_PORT_OUTSET1 = COLS[c];


            }
            //delay(10);
            REG_PORT_OUTCLR0 = col;
            delay(1);
        }
        /*ROWS &= ~ROW0;
        delay(500);
        COLUMNS &= ~COL0;
        delay(500);
        COLUMNS |= COL0;
        delay(500);
        ROWS |= ROW0;
        delay(500);
        COLUMNS &= ~COL0;
        delay(500);
        COLUMNS |= COL0;
        delay(500);*/
    }
}
