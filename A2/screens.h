#pragma once 
// only define if not already defined 
// ******** GET *********
// return X_MAX
int get_X_MAX(void);
// return X_MIN
int get_X_MIN(void); 
// return Y_MAX
int get_Y_MAX(void);


// ******** SET *********

// ********  SCREENS  **********
// display the game stats 
void draw_game_status(void); 
// do game status update
void do_game_status_update(void);
// draw the game screen 
void draw_game_screen(void); 
// draw quit screen 
void draw_quit_screen(void); 
// reset game 
void do_game_reset (void); 
// update game parameters 
void do_game_update(void); 
