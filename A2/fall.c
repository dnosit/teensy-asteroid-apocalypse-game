// STARFIGHTER 
//
// *********** Std system libs 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
// ********** CAB202 teensy folder libs 
#include <lcd.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>
#include <macros.h>    
#include "lcd_model.h" 
// ********** A2 other modules in this folder
#include "setup_teensy.h" 
#include "time.h"
#include "screens.h"
#include "deflector.h"
#include "starfi.h"
// initial number int array
#define ASTEROIDS_MAX (3)
#define BOULDERS_MAX (6) 
#define FRAGMENTS_MAX (12)
#define PLASMA_MAX (20)
// value of asteroid not counted 
#define NO_OBJECT (-2)

// ******** GLOBAL ************
// init object coords 
// bottom point y for falling objects 
int asteroid_x[ASTEROIDS_MAX] = {};
int asteroid_y[ASTEROIDS_MAX] = {}; 
int boulder_x[BOULDERS_MAX] = {};
int boulder_y[BOULDERS_MAX] = {};
int fragment_x[FRAGMENTS_MAX] = {};
int fragment_y[FRAGMENTS_MAX] = {};
// top left x for plasma
int plasma_x[PLASMA_MAX] = {};
int plasma_y[PLASMA_MAX] = {};

// ******** FUNCTIONS ***********
// 
// ******** DRAWING ***********

// draw fragment (plus shape) 
void draw_fragment(int x_pos, int y_pos) {
    // 3x3 pixels default
    // vertical line
    draw_line(x_pos, y_pos, x_pos, y_pos +3, FG_COLOUR);
    // horizontal line
    draw_line(x_pos -1, y_pos, x_pos +1, y_pos, FG_COLOUR); 
}

// draw diamond shape of given size and start pos
void draw_diamond(int x_pos, int y_pos, int size) {
    // initial x left and right the same point
    int x_pos_left = x_pos;
    int x_pos_right = x_pos; 
    // calc offset from middle 
    int offset = (int)(size/2); 
    // draw diamond BOTTOM half, starting at BOTTOM point
    for (int x = 0; x <= offset; x++) {
        //draw line up to middle
        draw_line(x_pos_left, y_pos, x_pos_right, y_pos, FG_COLOUR); 
        // move left x left one each time, opposite for right
        x_pos_left -= 1;
        x_pos_right += 1; 
        // step y to next line up
        y_pos--; 
    }
    // adjust xpos's so next line starts 1 in 
    x_pos_left += 1; 
    x_pos_right -= 1;
    // draw diamond bottom half, continuing
    for (int x = 0; x < offset; x++) {
        // move left x right one each time, opposite for right
        x_pos_left += 1;
        x_pos_right -= 1; 
        //draw line from middle up
        draw_line(x_pos_left, y_pos, x_pos_right, y_pos, FG_COLOUR); 
        // step y to next line up
        y_pos--; 
    }
} // end draw diamond 

// draw asteroid (from bottom x & y) 
void draw_asteroid(int x_pos, int y_pos) {
    // diamond of size 7
    draw_diamond(x_pos, y_pos, 7);
}
// draw boulder (from bottom x & y) 
void draw_boulder(int x_pos, int y_pos) {
    // diamond of size 5
    draw_diamond(x_pos, y_pos, 5);
}
// draw a plasma bolt from top left x 
void draw_plasma(int x_pos, int y_pos) {
    // square 2x2 pixels, starting at top left
    draw_line(x_pos, y_pos, x_pos +1, y_pos, FG_COLOUR); 
    // move one y down screen & draw another
    y_pos += 1; 
    draw_line(x_pos, y_pos, x_pos +1, y_pos, FG_COLOUR); 
}
// draw all plasma
void draw_plasma_all(void) {
    // for each asteroid in array
    for (int p = 0; p < PLASMA_MAX; p++) {
        // if greater than -2 for the y array, draw it
        if ( plasma_y[p] > NO_OBJECT ) {
            draw_plasma(plasma_x[p], plasma_y[p]);
        }
    } 
} // end 
// draw all asteroids 
void draw_asteroids_all(void) {
    // for each asteroid in array
    for (int ast = 0; ast < ASTEROIDS_MAX; ast++) {
        // if greater than -2 for the y array, draw it
        if ( asteroid_y[ast] > NO_OBJECT ) {
            draw_asteroid(asteroid_x[ast], asteroid_y[ast]);
        }
    }
} // end draw asteroids 

// draw all boulders 
void draw_boulders_all(void) {
    // for each asteroid in array
    for (int b = 0; b < BOULDERS_MAX; b++) {
        // if greater than -2 for the y array, draw it
        if ( boulder_y[b] > NO_OBJECT ) {
            draw_boulder(boulder_x[b], boulder_y[b]);
        }
    }
} //end 

// draw all fragments 
void draw_fragments_all(void) {
    // for each asteroid in array
    for (int f = 0; f < FRAGMENTS_MAX; f++) {
        // if greater than -2 for the y array, draw it
        if ( fragment_y[f] > NO_OBJECT ) {
            draw_fragment(fragment_x[f], fragment_y[f]);
        }
    } 
} //end 

// ****************************************
// ***********  ACTIONS  *******************

// reset asteroids new wave at random locations 
void reset_asteroids(void) {
    for (int ast = 0; ast < ASTEROIDS_MAX; ast++) {
        // get random x coord in bounds 
        int x_min, x_max; 
        x_min = get_X_MIN() + 4;
        x_max = get_X_MAX() - 4; 
        // calc rand x in bounds 
        int x = get_rand_int() % (x_max + x_min) + x_min; 
        asteroid_x[ast] = x; 
        // asteroid_y[ast] = -1; // 1 above top of screen 
    }
} // end reset asteroids 

// calc length of given array
int get_array_len( int *y_array ) {
    // divide bytes of array by bytes of one item to find length of array
    int array_len = sizeof(y_array) / sizeof(y_array[0]); 
    return array_len; 
}

// count objects remaining 
int get_object_count( int *y_array ) {
    // count var
    int count = 0; 
    // divide bytes of array by bytes of one item to find length of array
    int array_len = get_array_len(y_array); 
    for (int y = 0; y < array_len; y++ ) {
        // if greater than -2, count as 1
        if ( y_array[y] > -2 ) {
            count++; 
        } // end if 
    } // end for 
    return count;
} // end count objects 

// do respawn falling objects if needed 
void respawn_if_required(void) {
    // get count for each object
    int asteroids = get_object_count( asteroid_y ); 
    int boulders = get_object_count( boulder_y ); 
    int fragments = get_object_count( fragment_y );  
    // check if no objects left on screen
    if ( asteroids == 0 && boulders == 0 && fragments == 0 ) { // if all objects == 0 
        // only need to reset asteroids 
        reset_asteroids();  
    }
} // end respawn check 

// remove object x & y from given arrays
void do_remove_object(int x_array[], int y_array[], int index_remove) {
    // set index to -2 for given array 
    x_array[index_remove] = NO_OBJECT;
    y_array[index_remove] = NO_OBJECT; 
}

// if collision, removes object, reduces life etc. 
void do_collide_deflector(int x_array[], int y_array[], int index_remove) {
    // if hit deflector remove this object
    do_remove_object(x_array, y_array, index_remove);
    // decrease score by 1
    do_reduce_life(1);
    // check if respawn needed yet 
    respawn_if_required();
} 

// if collision, removes object, adds to score etc. 
void do_collide_plasma(int x_array[], int y_array[], int index_remove, int plasma_index, int score_add) {
    // if hit by plasma remove this object
    do_remove_object(x_array, y_array, index_remove); 
    // also remove the plasma bolt
    do_remove_object(plasma_x, plasma_y, plasma_index); 
    // incrase score by 1 if asteroid hit
    set_starfi_score_add(score_add); 
    // check if respawn needed yet
    respawn_if_required(); 
} 

// move asteroids
void do_move_asteroid(int y_add) {
    for (int ast = 0; ast < ASTEROIDS_MAX; ast++ ) {
        // only draw if:
        int next_pos = asteroid_y[ast] += y_add; 
        // IS VALID OBJECT && NOT COLLIDED WITH DEFLECTOR
        if ( asteroid_y[ast] > NO_OBJECT && !deflector_collision( next_pos ) ) {
            asteroid_y[ast] = next_pos;
        }
        // else, if collision, do collision 
        else if ( deflector_collision( next_pos ) ) {
            do_collide_deflector(asteroid_x, asteroid_y, ast); 
        }
    }
} // end 

// move boulders
void do_move_boulder(int y_add) {
    for (int b = 0; b < BOULDERS_MAX; b++ ) {
        // only draw if:
        int next_pos = boulder_y[b] + y_add; 
        // IS VALID OBJECT && NOT COLLIDED WITH DEFLECTOR
        if ( boulder_y[b] > NO_OBJECT && !deflector_collision( next_pos ) ) {
            boulder_y[b] = next_pos;
        }
        // else, if collision, do collision 
        else if ( deflector_collision( next_pos ) ) {
            do_collide_deflector(boulder_x, boulder_y, b); 
        }
    }
} // end 

// move fragments
void do_move_fragment(int y_add) {
    for (int f = 0; f < FRAGMENTS_MAX; f++ ) {
        // only draw if:
        int next_pos = fragment_y[f] + y_add; 
        // IS VALID OBJECT && NOT COLLIDED WITH DEFLECTOR
        if ( fragment_y[f] > NO_OBJECT && !deflector_collision( next_pos ) ) {
            fragment_y[f] = next_pos;
        } 
        // else, if collision, do collision 
        else if ( deflector_collision( next_pos ) ) {
            do_collide_deflector(fragment_x, fragment_y, f); 
        }
    }
} // end 

// find first "empty" index in array
int get_first_empty_index(int *array_y ) {
    // check for first "empty" place in array
    int index = 0; 
    while ( (array_y[index] > NO_OBJECT) && (index < get_array_len(array_y)) ) {
        index++; 
    }
    return index; 
}

// split asteroid into two boulders
// NB: independant of other removal of asteroid etc. 
void do_split_asteroid(int ast_x, int ast_y) {
    // for left boulder 
    int index = get_first_empty_index(boulder_y); 
    // if this index is also empty, add to it
    if ( boulder_y[index] <= NO_OBJECT ) {
        boulder_x[index] = ast_x - 5; 
        boulder_y[index] = ast_y;
    }
    // repeat for right boulder 
    index = get_first_empty_index(boulder_y); 
    // if this index is also empty, add to it
    if ( boulder_y[index] <= NO_OBJECT ) {
        boulder_x[index] = ast_x + 5; 
        boulder_y[index] = ast_y;
    }
} // end 

// split asteroid into two boulders
// NB: independant of other removal of asteroid etc. 
void do_split_boulder(int bold_x, int bold_y) {
    // for left boulder 
    int index = get_first_empty_index(fragment_y); 
    // if this index is also empty, add to it
    if ( fragment_y[index] <= NO_OBJECT ) {
        fragment_x[index] = bold_x - 5; 
        fragment_y[index] = bold_y;
    }
    // repeat for right boulder 
    index = get_first_empty_index(boulder_y); 
    // if this index is also empty, add to it
    if ( fragment_y[index] <= NO_OBJECT ) {
        fragment_x[index] = bold_x + 5; 
        fragment_y[index] = bold_y;
    }
} // end 

// shoot plasma (add to array)
void do_plasma(void) {
    // check for first "empty" place in plasma array
    int index = 0; 
    while ( (plasma_y[index] > NO_OBJECT) && (index < PLASMA_MAX) ) {
        index++; 
    }
    // if this index is "empty", add the plasma
    if (plasma_y[index] <= NO_OBJECT) {
        // add a new plasma bolt to the array
        // X, at current end of starfighter waveguide
        plasma_x[index] = get_starfi_x_current(); 
        // Y, less height of plasma(2) and 1 above deflector
        plasma_y[index] = ( get_DEFLECTOR_Y() -2 - 1 ); 
    }
    else { } // do nothing
} // end plasma shoot 

// checks for collision and takes action if so 
void plasma_collision_asteroid(int plasma_x, int plasma_y, int plasma_index) {
    // plasma coords along y given 
    int plasma_x1 = plasma_x; // top left plasma x
    int plasma_x2 = plasma_x1 + 1; // top right plasma x
    // check each object of given arrays of same length 
    for (int a = 0; a < ASTEROIDS_MAX; a++ ) {
        // check coords of bottom row of asteroid 
        for (int abot = 0; abot <= 4; abot++) {
            // asteroid bottom row coords 
            int x_left = asteroid_x[a] - abot; 
            int x_right = asteroid_x[a] + abot; 
            int y = asteroid_y[a] + abot; 
            // check against plasma
            // check if y matches first (for processing efficiency)
            if ( y == plasma_y ) {
                // then check x 
                if ( x_left == plasma_x1 || x_left == plasma_x2 || x_right == plasma_x1 || x_right == plasma_x2 ) {
                    // split the asteroid
                    do_split_asteroid(asteroid_x[a], asteroid_y[a]); 
                    // do collision
                    do_collide_plasma(asteroid_x, asteroid_y, a, plasma_index, 1); 
                }
                else; 
            }
        }
    }
} // end 

// checks for collision and takes action if so 
void plasma_collision_boulder(int plasma_x, int plasma_y, int plasma_index) {
    // plasma coords along y given 
    int plasma_x1 = plasma_x; // top left plasma x
    int plasma_x2 = plasma_x1 + 1; // top right plasma x
    // check each object of given arrays of same length 
    for (int b = 0; b < BOULDERS_MAX; b++ ) {
        // check coords of bottom row of boulder
        for (int bbot = 0; bbot <= 3; bbot++) {
            // asteroid bottom row coords 
            int x_left = boulder_x[b] - bbot; 
            int x_right = boulder_x[b] + bbot; 
            int y = boulder_y[b] + bbot; 
            // check against plasma
            // check if y matches first (for processing efficiency)
            if ( y == plasma_y ) {
                // then check x 
                if ( x_left == plasma_x1 || x_left == plasma_x2 || x_right == plasma_x1 || x_right == plasma_x2 ) {
                    // split the asteroid
                    do_split_boulder(boulder_x[b], boulder_y[b]); 
                    // do collision
                    do_collide_plasma(boulder_x, boulder_y, b, plasma_index, 2); 
                }
                else; 
            }
        }
    }
} // end 

// move plasma
void do_move_plasma(int y_add) {
    for (int p = 0; p < PLASMA_MAX; p++ ) {
        // only draw if:
        int plasma_y_next = plasma_y[p] -= y_add; 
        // check for collision and do collided if so 
        plasma_collision_asteroid( plasma_x[p], plasma_y_next, p); 
        plasma_collision_boulder(plasma_x[p], plasma_y_next, p); 
        // NB: if this has collided, will reset to no object
        if ( plasma_y[p] > NO_OBJECT ) {
            plasma_y[p] = plasma_y_next;
        }
        else; 
    }
} // end move 

// "remove" all plasma bolts from array
void clear_plasma(void) {
    for (int p = 0; p < PLASMA_MAX; p++) {
        do_remove_object(plasma_x, plasma_y, p); 
    }
} // end clear plasma 


// clear falling objects CLEAR = NO_OBJECT = -2 
void clear_falling_objects(void) {
    // CLEAR asteroids 
    for (int a = 0; a < ASTEROIDS_MAX; a++) {
        do_remove_object(asteroid_x, asteroid_y, a); 
    } 
    // CLEAR boulders
    for (int b = 0; b < BOULDERS_MAX; b++) {
        do_remove_object(boulder_x, boulder_y, b); 
    }
    // CLEAR fragments
    for (int f = 0; f < FRAGMENTS_MAX; f++) {
        do_remove_object(fragment_x, fragment_y, f); 
    }
}
