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
	configureP1PinAsButton(BIT1|BIT2|BIT3|BIT4);
	P1IE |= BIT1|BIT2|BIT3;                 // enable the interrupts
	P1IES |= BIT1|BIT2|BIT3;                 // configure interrupt to sense falling edges
	__enable_interrupt();
}

char flag = 0;
char winstring1[] = "YOU     ";
char winstring2[] = "WIN!    ";
char losestring1[] = "YOU     ";
char losestring2[] = "LOSE!   ";
char minestring1[] = "mine?   ";
char minestring2[] = "press up";
int isGameOver;
int player;
int direction;
int release;


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
        	player = 0;
        	flag = 0; //for interrupt
        	isGameOver = 1;
        	LCDclear();
        	TACTL &= ~(MC1|MC0);        // stop timer
        	TACTL |= MC1;

        	player = initPlayer();
        	char direction = 0;
        	printPlayer(player);
        	release = 1;


        while (isGameOver)
        {
        	if(player == 0xC7)
        	{
        		isGameOver = 0;
        		LCDclear();
        		writeString(winstring1);
        		cursorToLineTwo();
        		writeString(winstring2);
        		__delay_cycles(10000000);
        	}
        }


//
// YOUR TIMER A ISR GOES HERE
//

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR();
{
	TACTL &= ~TAIFG; //clear flag
	flag += 1; //taken from Colin's code
	if(flag == 4)
	{
		isGameOver = 0;
		LCDclear();
		writeString(losestring1);
		cursorToLineTwo();
		writeString(losestring2);
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void);
{
	if(P1IFG & BIT1) //are the flag and button down?
	{
		if((isP1ButtonPressed(BIT1)) && (player != 0) && (isGameOver == 1))
		{
			direction = RIGHT;
			player = movePlayer(player, direction);
			TAR = 0;
			flag = 0;
		}

		P1IFG &= ~BIT1; //clear the flag again

	}

	if(P1IFG & BIT2) //are the flag and button down?
		{
			if((isP1ButtonPressed(BIT2)) && (player != 0) && (isGameOver == 1))
			{
				direction = LEFT;
				player = movePlayer(player, direction);
				TAR = 0;
				flag = 0;
			}

			P1IFG &= ~BIT2; //clear the flag again

		}

	if(P1IFG & BIT3) //are the flag and button down?
		{
			if((isP1ButtonPressed(BIT3)) && (player != 0) && (isGameOver == 1))
			{
				direction = UP;
				player = movePlayer(player, direction);
				TAR = 0;
				flag = 0;
			}

			P1IFG &= ~BIT3; //clear the flag again

		}

	if(P1IFG & BIT4) //are the flag and button down?
		{
			if((isP1ButtonPressed(BIT4)) && (player != 0) && (isGameOver == 1))
			{
				direction = DOWN;
				player = movePlayer(player, direction);
				TAR = 0;
				flag = 0;
			}

			P1IFG &= ~BIT4; //clear the flag again

		}

		}



        }
}
