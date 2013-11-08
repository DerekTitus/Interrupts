Interrupts
==========
Lab 5 - LCD Game
This file contains the code for a simple LCD game. The player starts in the top left, as defined as an "*" and must navigate through the use of buttons to the bottom right while performing a movement at least once every two seconds.

Extra Files
This program utilizes libraries for LCD and Buttons.

Functions contained with code
unsigned char initPlayer() - initializes the player in top left.
void printPlayer() - prints the player where it was initialized (top left).
void clearPlayer() - clears the player from a position.
unsigned char movePlayer() - moves the player up, right, down, and left depending on which button is pressed. 
char didPlayerWin() - checks to see if the player is at the right spot. If they are, then they win. 
