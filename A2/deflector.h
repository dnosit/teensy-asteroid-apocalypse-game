#pragma once 
// only define if not already defined 

// return deflector y
int get_DEFLECTOR_Y(void);
// returns lives 
int get_lives(void);
// reset lives 
void reset_lives(void); 
// reduce lives by amount
void do_reduce_life(int amount); 
// reset deflector 
void do_deflector_reset(void); 
// draw deflector 
void draw_deflector(void); 

// ************** COLLISIONS **************
// retruns true if collision with deflector 
bool deflector_collision(int obj_y_bottom );
// do game over actions 
void do_game_over(void); 
// return true if game over 
bool game_over(void); 