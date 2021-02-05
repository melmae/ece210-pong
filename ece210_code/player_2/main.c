//*****************************************************************************
// main.c
// Author: Melissa Schmidt and Alexander Ulate
//*****************************************************************************
#include "lab_buttons.h"
/******************************************************************************
 * Global Constants and Variables
 *****************************************************************************/
#define MOVE_PIXELS   	1							// Number of Pixels to move each time.
#define LCD_SIZE_X			240						// X Size of LCD screen in pixels
#define LCD_SIZE_Y			320						// Y Size of LCD screen in pixels
#define X_START					105  					// Starting position of the paddle
#define Y_START					300
#define X_PADDLE				30  					// Paddle size
#define Y_PADDLE				5
#define X_BALL					5							// Ball size
#define Y_BALL					5
#define X_BALL_START		115						// Ball start spot
#define Y_BALL_START		155
#define WALL_R					0x00					//Ball direction options
#define WALL_L					0x01
#define TOP							0x02
#define BOTTOM					0x03
#define PADDLE					0x04
#define NEW_TURN				0x05					// Send the direction to default
#define LOCAL_ID				0x33					// Local ID
#define REMOTE_ID				0x32				// Remote ID


//*****************************************************************************
//	PLAYER 2 CODE
//*****************************************************************************
int 
main(void)
{
  uint32_t 	x_pos = X_START;		// x_pos holds the x position of the right corner of the image
	uint32_t	round_over = 500;		// the message for game over
	uint32_t	game_start = 600;		// the message for game start
	uint32_t	message_check;			// Check to see what the message contains
	uint8_t 	direction;					// The direction of the joystick
	uint8_t		ballDirection;			// The ball's direction for the switch case
	bool			ballxDirection = 0;	// The ball's X direction
	bool			ballyDirection = 1; // The ball's Y direction
	uint32_t	ball_pos_x = X_BALL_START;	// The position of the ball in the X dimension
	uint32_t	ball_pos_y = Y_BALL_START;	// The position of the ball in 
	bool			wireless_data = 0;	// The check to see if the wireless revieved starts the game
	bool			data_send = 0;
	uint32_t  turn = 0; // Set to 1 for player 1 and 0 for player 2
	int 			test = 0;	
	char 			msg[30];
	int 			start;
	uint8_t		game_over = 0;
	uint32_t 	angle = 1;
	
	// Setting up the board
  ece210_initialize_board(); // Set up the wireless
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	ece210_lcd_print_string("2-Player Wireless PONG", 215, 100, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
	ece210_lcd_print_string("Don't let the ball", 215, 150, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
	ece210_lcd_print_string("touch the bottom!", 215, 170, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
	ece210_lcd_print_string("Press any button to start", 215, 300, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
	
	if(turn){
		start = 1;
	}
	else {
		start = 0;
		ece210_lcd_print_string("Press any button to start", 215, 300, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
	}
	
	while (1) {
		
		if ((turn == 0) && (game_over == 0)){	// Display the waiting screen
			if (test!= 100){
				test = test + 1;
				if (test == 1){
					ece210_lcd_print_string("Waiting for other player   ", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_GREEN);
				}
				if (test == 33){
					ece210_lcd_print_string("Waiting for other player.", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_GREEN);
				}
				if (test == 66){
					ece210_lcd_print_string("Waiting for other player..", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_GREEN);
				}
					
			} 
			else {
				ece210_lcd_print_string("Waiting for other player...", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_GREEN);
				sprintf(msg,"%d\n", ball_pos_x); // This code is necessary to allow the wireless to transmit correctly
				ece210_lcd_print_string(msg,0,0,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
				ece210_wait_mSec(20);
				test = 0;
			}
	}

		
		if (ece210_wireless_data_avaiable()) { // When it recieves the message from the other board
			message_check = ece210_wireless_get();
			if (message_check == round_over){
				game_over = 2;
			}
			else if (message_check == game_start){
				ece210_lcd_print_string("2-Player Wireless PONG", 215, 100, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("Don't let the ball", 215, 150, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("touch the bottom!", 215, 170, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("GAME START!", 170, 170, LCD_COLOR_GREEN, LCD_COLOR_BLACK);
				ece210_wait_mSec(700);
				ece210_lcd_print_string("GAME START!", 170, 170, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
			} else {
				turn = 1;
				ece210_lcd_print_string(" Waiting for other player...", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_BLACK); // clear the words
				ece210_lcd_print_string("2-Player Wireless PONG", 215, 100, LCD_COLOR_BLACK, LCD_COLOR_BLACK); // clear words in case the player didn't reset before Game Start message
				ece210_lcd_print_string("Don't let the ball", 215, 150, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("touch the bottom!", 215, 170, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("Ready?", 140, 200 , LCD_COLOR_RED, LCD_COLOR_BLACK);
				ece210_wait_mSec(500);
				ece210_lcd_print_string("Ready?", 140, 200 , LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ball_pos_y = 0;
				ballyDirection = 1;
				ball_pos_x = ece210_wireless_get();
				wireless_data = 1;	
			}
			
		}
		// Check to see if it is the end of the game 
		if (game_over == 1) { // Game over, you lost
			break;
		} else if (game_over == 2) { // Game over, you won
			ece210_lcd_draw_rectangle  (0, LCD_SIZE_X, 0, LCD_SIZE_Y, LCD_COLOR_BLACK); // Clear any remaining images
			ece210_lcd_print_string("GAME OVER",165,70,LCD_COLOR_RED, LCD_COLOR_BLACK);
			ece210_lcd_print_string("YOU WIN!",160,150,LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
			ece210_lcd_print_string(" Waiting for other player...", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_BLACK);
			ece210_lcd_print_string("Press RESET to play again", 215, 300, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
			ece210_wait_mSec(1);
			break;
		}
		
		wireless_data = 1;
	
  while(wireless_data)
  {

			while(start) {
				if(AlertButtons) {
					AlertButtons = false;
					if (btn_down_pressed() || btn_left_pressed() || btn_right_pressed() || btn_up_pressed()){
						start = 0;
						ece210_lcd_print_string("Press any button to start", 215, 300, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
						ece210_lcd_print_string("2-Player Wireless PONG", 215, 100, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
						ece210_lcd_print_string("Don't let the ball", 215, 150, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
						ece210_lcd_print_string("touch the bottom!", 215, 170, LCD_COLOR_BLACK, LCD_COLOR_BLACK);
						ece210_lcd_draw_rectangle  (X_START, X_PADDLE, Y_START, Y_PADDLE, LCD_COLOR_WHITE);
						ece210_lcd_draw_rectangle  (X_BALL_START, X_BALL, Y_BALL_START, Y_BALL, LCD_COLOR_WHITE);
					}
				}
			}
			
			if (turn) {
				ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, Y_BALL, LCD_COLOR_WHITE);
			}
			
			direction = ece210_ps2_read_position();		// Joystick function to read digital value of x or Y pointing.
																								// One of Up, Down, Left or Right value returned.
			
			switch (direction)												// based on the joystick direction move the image.
			{
				case PS2_RIGHT:													// Joystick pointing left.ll
				{
					if((x_pos+30) < LCD_SIZE_X - MOVE_PIXELS)	// if not moving past the left edge
					{
							ece210_lcd_draw_rectangle  (x_pos, MOVE_PIXELS, Y_START, Y_PADDLE, LCD_COLOR_BLACK); // erase part of image
						x_pos = x_pos + MOVE_PIXELS;																														// that won't be redrawn
					}										
					break;
				}
				case PS2_LEFT:
				{
					if(x_pos > MOVE_PIXELS-1)														// if not moving past the right edge
					{
						ece210_lcd_draw_rectangle  (x_pos+X_PADDLE-MOVE_PIXELS, MOVE_PIXELS, Y_START, Y_PADDLE, LCD_COLOR_BLACK);
						x_pos = x_pos - MOVE_PIXELS;
					}
					break;
				}

				case PS2_CENTER:
				{
					// Do nothing
					break;
				}
				default:
				{
					break;
				}
			}	// end direction switch
			ece210_lcd_draw_rectangle  (x_pos, X_PADDLE, Y_START, Y_PADDLE, LCD_COLOR_WHITE);
			
			if (turn) {
			
		// Ball Movement
		if(ball_pos_x >= 240) // Did the ball hit the right wall?
		{
			ballDirection = WALL_R;
		} else if (ball_pos_x <= 1)	// Did the ball hit the left wall?
		{
			if (angle == 2){
				ball_pos_x = 2;
				ece210_wait_mSec(10);
			}
			ballxDirection = 1;
			ece210_lcd_draw_rectangle  (0, LCD_SIZE_X, ball_pos_y, Y_BALL, LCD_COLOR_BLACK);
			ballDirection = WALL_L;
		} else if (ball_pos_y >= 320)	// Did the ball hit the bottom?
		{
			ballDirection = BOTTOM;
		} else if ((ball_pos_y <= 0) && (ballyDirection == 0))	// Did the ball hit the top?
		{
			ballDirection = TOP;
		} else if ((ball_pos_y + 5) == Y_START && (ball_pos_x >= x_pos && ball_pos_x <= (x_pos + 30)))	// Did the ball hit the paddle?
		{ // 5 total options of ways to bounce the ball
			if (ball_pos_x < x_pos + 5){					// Left side bounces left at angle 2
				angle = 2;
			} else if (ball_pos_x > x_pos + 23) { // Right side bounce right at angle 2
				angle = 2;
			} else {														// Left, middle, and Right bounce at angle 1
				angle = 1;
			}
			ece210_lcd_draw_rectangle  (0, LCD_SIZE_X, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
			ballDirection = PADDLE;
		}
		
		switch (ballDirection)	// Direction of the ball based on what it hit
		{
			case WALL_R:
			{
				// Bounce left
				ballxDirection = 0;
				
				if (ballyDirection){
					ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, MOVE_PIXELS, LCD_COLOR_BLACK);
					ball_pos_y = ball_pos_y + MOVE_PIXELS;
				} else {
					ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, (MOVE_PIXELS*angle)+angle, ball_pos_y, Y_BALL + 1, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - (MOVE_PIXELS*angle);
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				}
				break;
			}
			case WALL_L:
			{
				// Bounce right
				ballxDirection = 1;
				if (ballyDirection){
					ece210_lcd_draw_rectangle  (ball_pos_x, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x + MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, MOVE_PIXELS, LCD_COLOR_BLACK);
					ball_pos_y = ball_pos_y + MOVE_PIXELS;
				} else {
					ece210_lcd_draw_rectangle  (ball_pos_x, (MOVE_PIXELS*angle)+angle, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x + (MOVE_PIXELS*angle);
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, (MOVE_PIXELS*angle), LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				}
				break;
			}
			case TOP:
			{
				//// Wireless transfer /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				wireless_data = 0;
				data_send = 1;
				ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, Y_BALL, LCD_COLOR_BLACK);
				turn = 0;
				ballDirection = NEW_TURN;
				angle = 1;
				break;
			} ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case BOTTOM:
			{
				// Round over
				ece210_lcd_draw_rectangle  (0, LCD_SIZE_X, 0, LCD_SIZE_Y, LCD_COLOR_BLACK); // Clear any remaining images
				ece210_lcd_print_string("GAME OVER",165,70,LCD_COLOR_RED, LCD_COLOR_BLACK);
				ece210_lcd_print_string("YOU LOOSE!",165,150,LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
				ece210_lcd_print_string(" Waiting for other player...", 220, 200 , LCD_COLOR_BLACK, LCD_COLOR_BLACK);
				ece210_lcd_print_string("Press RESET to play again", 215, 300, LCD_COLOR_BLUE, LCD_COLOR_BLACK);
				ece210_wait_mSec(1);
				ece210_wireless_send(round_over);
				turn = 0;
				game_over = 1;
				break;
			}
			case PADDLE:
			{
				ballyDirection = 0;
				// Bounce depending on position on the paddle
				if (ball_pos_x < x_pos +10)					// Left side bounces left
				{	
					ballxDirection = 0;
					ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, (MOVE_PIXELS*angle)+angle, ball_pos_y, Y_BALL + 1, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - (MOVE_PIXELS*angle);
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				} else if ((ball_pos_x >= x_pos + 10) && (ball_pos_x <= x_pos + 20) ) { // Middle bounce
					if (ballxDirection == 0) { // Continue moving left
						ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
						ball_pos_x = ball_pos_x - (MOVE_PIXELS*angle);
						ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
						ball_pos_y = ball_pos_y - MOVE_PIXELS;
						break;
					} else{ // Continue moving right
						ece210_lcd_draw_rectangle  (ball_pos_x, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
						ball_pos_x = ball_pos_x + (MOVE_PIXELS*angle);
						ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
						ball_pos_y = ball_pos_y - MOVE_PIXELS;
						break;
						
					}
				} else {														// Right side bounces right
					ballxDirection = 1;
					ece210_lcd_draw_rectangle  (ball_pos_x, (MOVE_PIXELS*angle)+angle, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x + (MOVE_PIXELS*angle);
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				}
				break;
			}
			default: {
				// Used when the ball is being returned
				if (ballxDirection && ballyDirection)
				{
					ece210_lcd_draw_rectangle  (ball_pos_x, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
          ball_pos_x = ball_pos_x + MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y + MOVE_PIXELS;
				} else if (ballxDirection) 
				{
					ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				} else if (ballyDirection)
				{
					ece210_lcd_draw_rectangle  (ball_pos_x+X_BALL-MOVE_PIXELS, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y + MOVE_PIXELS;
				} else {
					ece210_lcd_draw_rectangle  (ball_pos_x, MOVE_PIXELS, ball_pos_y, Y_BALL, LCD_COLOR_BLACK); // erase part of image
					ball_pos_x = ball_pos_x - MOVE_PIXELS;
					ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y+Y_BALL-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          ball_pos_y = ball_pos_y - MOVE_PIXELS;
				}
				break;
			}
			}
			if (turn) {
				ece210_lcd_draw_rectangle  (ball_pos_x, X_BALL, ball_pos_y, Y_BALL, LCD_COLOR_WHITE);
			}
			
	} // end of turn while loop
		

  
	ece210_wait_mSec(10);
	if (turn == 0) {
		wireless_data = 0;
	}
}
	if (data_send) {
	ece210_wireless_send(ball_pos_x);
	ece210_wait_mSec(10);
		data_send = 0;
	}
	
	}	// end while 1
}	// end main
