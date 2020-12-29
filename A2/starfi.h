#pragma once 
// only define if not already defined 


// returns score
int get_starfi_score(void);
// returns starfi_move_auto
int get_starfi_x_dir(void); 
// get current starfi x
int get_starfi_x_current(void); 
// set starfi score 
void set_starfi_score_add(int add_amount); 
// update the starfi_move_auto var 
void set_starfi_x_dir(int auto_set); 
// reset starfighter parameters
void do_starfi_reset(void);
// cannon turret to change by angle
void draw_cannon(int x_start, int y_start);
// draw the starfighter 
void draw_starfi(void); 
// move the starfighter left / right given amount
void do_starfi_move(int move_x_dir); 

