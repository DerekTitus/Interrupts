/*
 * Interrupts.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Derek.Titus
 */



#include <msp430g2553.h>
#include "Interrupts.h"


unsigned char initPlayer()
{
        return 0x80;
}

void printPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction)
{
        switch (direction) {
                //
                // update player position based on direction of movement
                //
        case RIGHT:
        		clearPlayer(player);
        			player += 1;
        			if (player == 0x88)
        			{
        				player = 0x87; // 87 is the right limit/it cannot be 88
        			}
        			if (player ==0xC8)
        			{
        				player = 0xC7; //c7 is the right limit
        			}
        			printPlayer(player);
        			waitForP1ButtonRelease(BIT1); //this function was taken from C2C Busho's code
        break;
        case LEFT:
        		clearPlayer(player);
        			player -= 1;
        			if (player == 0x7F)
        			{
        				player = 0x80;
        			}
        			if (player == 0xBF)
        			{
        				player = 0xC0;
        			}
        			printPlayer(player);
        			waitForP1ButtonRelease(BIT2); //bit2 is connected to left button
        break;
        case UP:
        		clearPlayer(player);
        			if (player >= 0xC0) //if it is in the bottom line
        			{
        				player -= 0x40; //the difference between C and 8 is 4. This also keeps the horizontal placement the same
        			}
        			printPlayer(player);
        			waitForP1ButtonRelease(BIT3); //bit 3 is connected to up button
        break;
        case DOWN:
        		clearPlayer(player);
        			if (player <= 0x87)
        			{
        				player -= 0x40; //same as before, difference between C and 8 is 4.
        			}
        			printPlayer(player);
        			waitForP1ButtonRelease(BIT4); //bit 4 is down button
        break;


        			}
        			        }

        return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;
}
