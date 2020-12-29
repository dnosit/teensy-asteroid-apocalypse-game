#pragma once 
// only define if not already defined 

// ******** DRAWING ***********
// draw diamond shape of given size and start pos
void draw_diamond(int x_pos, int y_pos, int size); 
// draw asteroid (from bottom x & y) 
void draw_asteroid(int x_pos, int y_pos);
// draw boulder (from bottom x & y) 
void draw_boulder(int x_pos, int y_pos);
// draw a plasma bolt from top left x 
void draw_plasma(int x_pos, int y_pos);
// draw all plasma
void draw_plasma_all(void);
// draw all asteroids 
void draw_asteroids_all(void); 
// draw all boulders 
void draw_boulders_all(void); 
// draw all fragments 
void draw_fragments_all(void); 

// GET 
// count objects remaining 
int get_object_count(int y_array[]);
// calc length of given array
int get_array_len( int *y_array ); 

// SET
// set starfi score 
void set_starfi_score_add(int add_amount); 

// ****************************************
// ***********  ACTIONS  *******************
// draw fragment (plus shape) 
void draw_fragment(int x_pos, int y_pos); 
// remove asteroid at index
void do_remove_asteroid(int index); 
// move asteroids
void do_move_asteroid(int y_add);
// move boulders
void do_move_boulder(int y_add); 
// move fragments
void do_move_fragment(int y_add); 
// find first "empty" index in array
int get_first_empty_index(int *array_y );
// split asteroid into two boulders
// NB: independant of other removal of asteroid etc. 
void do_split_asteroid(int ast_x, int ast_y); 
// split asteroid into two boulders
// NB: independant of other removal of asteroid etc. 
void do_split_boulder(int bold_x, int bold_y); 
// shoot plasma (add to array)
void do_plasma(void); 
// checks for collision and takes action if so 
void plasma_collision_asteroid(int plasma_x, int plasma_y, int plasma_index);
// checks for collision and takes action if so 
void plasma_collision_boulder(int plasma_x, int plasma_y, int plasma_index); 
// move plasma
void do_move_plasma(int y_add);

// if collision, removes asteroid, reduces life etc. 
void do_collide_deflector(int index_x, int index_y);
// if collision, removes object, adds to score etc. 
void do_collide_plasma(int x_array[], int y_array[], int index_remove, int plasma_index, int score_add); 
// reset asteroids new wave
void reset_asteroids(void);
// do respawn check
void respawn_if_required(void); 
// "remove" all plasma bolts from array
void clear_plasma(void);
// clear falling objects CLEAR = NO_OBJECT = -2 
void clear_falling_objects(void); 
