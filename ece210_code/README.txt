*************************************************************************************************
* READ ME File											*
*************************************************************************************************
* Authors: Melissa Schmidt and Alexander Ulate							*
*************************************************************************************************
Instructions on how the program works, what is in the zip file,
how to use the program with the board, and what lessons you learned from the project.
/////////////////////////////////////////////////////////////////////////////////////////////////
// Program Instructions:								       //
/////////////////////////////////////////////////////////////////////////////////////////////////

The program has two seperate codes, one for the first player and one for the second player.
The game starts on the first player and the turn is set to 1.
The second player has its turn set to 0 and waits for the first player to send it data.
As either of the players wait, a green message is played telling the player that it is waiting 
for the other player to complete their task.

When player 1 presses a button on the board, the game starts with the ball traveling downward. 
This will also send a signal telling player 2 the game has started.

To move the paddle, the player uses the joystick to move it left and right.
The paddle has five different spots on it, allowing for different actions of the ball bouncing 
off of the paddle:
	- If the ball hits the far left side of the paddle, the ball bounces to the left and 
	  upward at an angle of about 30 degrees to the horizontal.
	- If the ball hits the left side closer to the middle, the ball bounces to the left and 
	  upward at an angle of about 45 degrees to the horizontal.
	- If the ball hits the middle of the paddle, the ball will bounce off at an angle of 45 
	  degrees to the horizontal in which ever direction it was previously traveling.
	- If the ball hits the right side closer to the middle, the ball bounces to the right 
	  and upward at an angle of about 45 degrees to the horizontal.
	- If the ball hits the far right side of the paddle, the ball bounces to the right and 
	  upward at an angle of about 30 degrees to the horizontal.

When a board recieves the position of the ball, the turn is set to 1, clears the green waiting 
message, and sends a warning signal to the player to get ready before starting.

Actions that the ball performs are as follows:
	- When the ball hits either of the side walls, it will bounce back at the same angle and 
	  vertical direction, with the horizontal direction becoming flipped.
	- If the ball hits the bottom of the screen, the game is over and will send a signal to 
	  the other board telling it that it won.
	- If the ball hits the top of the screen, the board will send the position to the other 
	  board and set turn to 0, waiting to recieve a signal back. This will in turn start 
	  playing the green waiting message from before.
	- When the position of the ball is recieved by the other player, the ball begins in that 
	  position, traveling in the same direction in the horizontal, but downwards at a default 
	  angle of 45 degrees.

When the game is completed and game_over is set to 1 or 2, the game loop ends.

/////////////////////////////////////////////////////////////////////////////////////////////////
// How to use this program with the board:					  	       //
/////////////////////////////////////////////////////////////////////////////////////////////////

First, there must be two boards in order to make the program work. One of the board must be
designated player 1 by uploading the player 1 program onto it. The other board must be designated
player 2 by uploading the player 2 program onto it. To interact with this program, the user needs 
only any of the four buttons on the board and the joystick. To start the game, the board with the 
player 1 code presses any of the four buttons. To move the paddle, use the joystick. When the 
game ends, both programs will finish. To replay the game, both boards must press the reset button,
with board one starting the game again. In order for the game to still function, player 2 must
reset their board before the player 1 board sends the signal for the ball position.

/////////////////////////////////////////////////////////////////////////////////////////////////
// Zip file content:								      	       //
/////////////////////////////////////////////////////////////////////////////////////////////////

This zip file contains:
	- the folder for player 1 code
		- main.c
		- project file for player 1
		- library folders
	- the folder for player 2 code
		- main.c
		- project file for player 2
		- library folders
	- this README file

/////////////////////////////////////////////////////////////////////////////////////////////////
// What we learned:								      	       //
/////////////////////////////////////////////////////////////////////////////////////////////////

We learned that the wireless on the boards does not work well in a switch case. We also learned 
that initializing the board will clear everything from the screen and reset the wireless in the 
process. We also learned that you need to keep track of what each of your variables are doing 
and sending all the time to avoid creating unexpected results in loops or in what is sent through 
wireless. We also learned that the processor and wireless are not instantaneous and require time
to complete their computations. It is because of this fact that we did not implement an angle
signal, becuase that would have to be sent at or near the same time as the signal for the
position of the ball. This would have slowed down our process and could cause errors if the
signals did not recieve in time before the game started again. One last lesson we learned was
to be careful of how you implement bounds and erasing previous images on the screen. When the
bounds for the screen were just one pixel off, it compounded as the ball traveled accross the
screen, leaving trails everywhere. Before finishing the code, the bounds must be figured out
to avoid and errors in trails or images dissapearing from the screen.
