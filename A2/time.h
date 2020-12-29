#pragma once 
// only define if not already defined 

// SET TIME / STATE 
// set intro state 
void set_intro(bool intro_set); 
// pause set 
void set_paused(bool pause_set);
// quit state set 
void set_quit(bool quit_set);
// quit toggle
void set_quit_toggle(void);
// set display status 
void set_stat_update(bool update); 
// pause toggle
void set_paused_toggle(void);
// set the elapsed time for the game 
void set_time_game_start(void);
// set the elapsed time for the game 
void reset_time_game_start(void);  
// set Game Speed
void set_game_speed(int game_speed_set);


// GET TIME / STATE 
// returns intro state
bool get_intro_state(void);
// returns game paused state 
bool get_game_state_paused(void); 
// returns quit state
bool get_game_state_quit(void); 
// returns status state
bool get_stat_update(void); 
// get elapsed time of game 
double get_time_game_start(void); 
// return Game Speed
int get_game_speed(void);
// returns elapsed time 
double get_elapsed_time_teensy(void); 
// get elapsed time of game 
double get_elapsed_time_game(void);



// ****  Helper functions ****

// see rand num generator 
void seed_rand_int(void); 
// get random int
int get_rand_int(void); 

// Helper drawing functions
void draw_double(uint8_t x, uint8_t y, double value, colour_t colour); 
void draw_int(uint8_t x, uint8_t y, int value, colour_t colour); 

// 