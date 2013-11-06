/*
 * Author: C2C Derek Titus
 * Is it faster?
 */

#include <msp430.h>
#include "Interrupts.h"
#include "buttons/button.h"
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
int isGameOver;


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
//        	char mine1 = 0;
//        	char mine2 = 0;
        	flag = 0; //for interrupt
        	isGameOver = 1;
        	LCDclear();
        	TACTL &= ~(MC1|MC0);        // stop timer
   //     	mine1 = generateMines(0, minestring1, minestring2);
  //      	mine2 = generateMines(mine1, minestring1, minestring2);
        	LCDclear();
//        	writeCommandByte(mine1);
//        	writeDataByte('X'); //puts an x where the mine1 is
//        	write CommandByte(mine2);
//        	writeDataByte('X');
        	TACTL |= MC1;

        	unsigned char player = initPlayer();
        	char direction = 0;
        	printPlayer(player);
        	char release = 1;

        	while (isGameOver)
        	{
        		if(isP1ButtonPressed(BIT1)) // Got help from Colin on this part and then used that knowledge to do the rest
        		{
        			direction = RIGHT;
        			player = movePlayer(player, direction); // goes into the RIGHT interrupt
        			TAR = 0;
        			flag = 0; //reset the flag
        		}
        		if(isP1ButtonPressed(BIT2))
        		{
        			direction = LEFT;
        			player = movePlayer(player, direction);
        			TAR = 0;
        			flag = 0;
        		}

        		if(isP1ButtonPressed(BIT3))
        		{
        			direction = UP;
        			player = movePlayer(player, direction);
        			TAR = 0;
        			flag = 0;
        		}

        		if (isP1ButtonPressed(BIT4))
        		{
        			direction = DOWN;
        			player = movePlayer(player, direction);
        			TAR = 0;
        			flag = 0;
        		}



        		}

        	while (release)
        		flag = 0;//need to clear the flag
        	{
        		if(isP1ButtonPressed(BIT1))
        		{
        			release = 0; //don't do anything until the button is no longer pressed
        			ButtonRelease(BIT1);
        		}

        		if(isP1ButtonPressed(BIT2))
        		{
        			release = 0;
        			ButtonRelease(BIT2);
        		}

        		if(isP1ButtonPressed(BIT3))
        		{
        			release = 0;
        			ButtonRelease(BIT3);
        		}

        		if(isP1ButtonPressed(BIT4))
        		{
        			release = 0;
        			ButtonRelease(BIT4);
        		}

        	}

        	LCDclear();

        		}
        	}
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





//
// YOUR TIMER A ISR GOES HERE
//

#pragma vector = TIMER_A_VECTOR
__interrupt void TIMER_A_ISR()
{
	TACTL &= ~TAIFG; //clear flag
	flag += 1; //taken from Colin's code
	if(flag == 4)
	{
		isGameOver = 0;
		LCDclear();
		writeString(losestring1, 8);
		cursorToLineTwo();
		writeString(losestring2, 8);
	}
}



