/*
 * Author: C2C Derek Titus
 * Is it faster?
 */

#include <msp430.h>
#include "Interrupts.h"
#include "button/buttons.h"
#include "LCD/LCD.h"

void init_timer()
{
	 TACTL &= ~(MC1|MC0);        // stop timer

	    TACTL |= TACLR;             // clear TAR

	    TACTL |= TASSEL1;           // configure for SMCLK - what's the frequency (roughly)?

	    TACTL |= ID1|ID0;           // divide clock by 8 - what's the frequency of interrupt?

	    TACTL &= ~TAIFG;            // clear interrupt flag

	    TACTL |= MC1;               // set count mode to continuous

	    TACTL |= TAIE;              // enable interrupt

	    __enable_interrupt();       // enable maskable interrupts
}


}
void init_buttons()
{
	P1IE |= BIT1|BIT2|BIT3;                 // enable the interrupts
	P1IES |= BIT1|BIT2|BIT3;                 // configure interrupt to sense falling edges

}

char flag = 0;
char winstring1[] = "YOU     ";
char winstring2[] = "WIN!    ";
char losestring1[] = "YOU     ";
char losestring2[] = "LOSE!   ";
char minestring1[] = "mine?   ";
char minestring2[] = "press up";

int main(void)
{
        WDTCTL = (WDTPW|WDTHOLD);

        unsigned char player = initPlayer();

        init_timer();
        init_buttons();
        __enable_interrupt();
        initSPI();
        LCDinit();


        while(1)
        {
        	char mine1 = 0;
        	char mine2 = 0;
        	flag = 0; //for interrupt
        	isGameOver = 1;
        	LCDclear();
        	TACTL &= ~(MC1|MC0);        // stop timer
        	mine1 = generateMines(0,minestring1, minestring2);
        	mine2 = generateMines(mine1, minestring1, minestring2);
                /*
                 * while (game is on)
                 * {
                 *                 check if button is pushed (you don't want to block here, so don't poll!)
                 *                 if button is pushed:
                 *                         clear current player marker
                 *                         update player position based on direction
                 *                         print new player
                 *                         clear two second timer
                 *                         wait for button release (you can poll here)
                 * }
                 *
                 * print game over or you won, depending on game result
                 *
                 * wait for button press to begin new game (you can poll here)
                 * wait for release before starting again
                 */
        }

        return 0;
}

//
// YOUR TIMER A ISR GOES HERE
//

void init_timer()
{
        // do timer initialization work
}

void init_buttons()
{
        // do button initialization work
}
