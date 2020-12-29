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
#include "fall.h"

// starfighter 
#define STARFIGHTER_HEIGHT (6) 
#define INITIAL_SCORE (0)
// ********* GLOBALS ************
// initial the starfighter spaceship at centre screen 
int starfi_x; 
int starfi_x_dir = 0; 
// turret angle initially 0 
int starfi_turret_angle = 0;
int score = INITIAL_SCORE; 

// ************* FUNCTIONS ********************
//GET
// returns score
int get_starfi_score(void) {
    return score; 
}
// returns starfi_move_auto
int get_starfi_x_dir(void) {
    return starfi_x_dir; 
}
// get current starfi x
int get_starfi_x_current(void) {
    return starfi_x; 
}

// SET
// set starfi score 
void set_starfi_score_add(int add_amount) {
    score += add_amount; 
}

// update the starfi_move_auto var 
void set_starfi_x_dir(int x_set) {
    starfi_x_dir = x_set; 
}

// reset starfighter parameters
void do_starfi_reset(void) {
    // initiallly the starfighter spaceship at centre screen 
    starfi_x = round(get_X_MAX() / 2); 
    // turret angle initially 0 
    starfi_turret_angle = 0; 
    // reset score 
    score = INITIAL_SCORE; 
    // set starfi move auto -1 or 1 based on mod 2 time elapsed 
    // if rand_int % 2 1, set 1 
    if ( get_rand_int() % 2 == 1 ) set_starfi_x_dir(1);
    // else set -1 
    else set_starfi_x_dir(-1); 
}

// cannon turret to change by angle
void draw_cannon(int x_start, int y_start) {
    // calculate top x from angle 
        // TO BE IMPLIMENTED 
    // adjust top x for angle of turret 
    int x_top = x_start; 
    // calc y start 
    int y_top = y_start - (STARFIGHTER_HEIGHT / 2 -1);
    // draw line, starting at the starfigher x, and  y - 1, going up
    draw_line(x_start, y_start, x_top, y_top, FG_COLOUR); 
} // end draw cannon 

// draw the starfighter 
void draw_starfi(void) {
    int starfi_y = get_Y_MAX() - STARFIGHTER_HEIGHT / 2; 
    // draw the linear wave guide (bottom always starts at starfighter top, drawing up)
    draw_cannon(starfi_x, starfi_y -1); 
    // starfighter pos offset vars inti to start 
    int x_left = starfi_x -1,  x_right = starfi_x + 1; 
    // increment y to increase width towards base
    for (int y = starfi_y;  y < starfi_y + STARFIGHTER_HEIGHT / 2;  y++ ) {
        // increase width toward base
        x_left-=1;
        x_right+=1; 
        // draw line 
        draw_line(x_left, y, x_right, y, FG_COLOUR);
    } 
    // move x left & right out 1 more pixel to draw wings
    x_left-=1;
    x_right+=1; 
    // draw wings 
    // left 
    draw_line(x_left, starfi_y, 
                x_left, get_Y_MAX(), FG_COLOUR);
    // right
    draw_line(x_right, starfi_y, 
                x_right, get_Y_MAX(), FG_COLOUR);
} // end draw star_fi

// move the starfighter left / right given amount
void do_starfi_move(int move_x_dir) {
    // adjustment offset factor from centre
    int offset = (STARFIGHTER_HEIGHT / 2) + 1 + 1; 
        // drawing width + start width (1)  + wings (1) 
    int starfi_x_next = starfi_x + move_x_dir; 
    // adjust starfighter_x if in bounds of screen 
    if ( (starfi_x_next - offset) > get_X_MIN()  &&  (starfi_x_next + offset) < get_X_MAX() ) {
        // check move direction 
        // if move right
        if ( starfi_x_next > starfi_x ) {
            // move positive direction 
            starfi_x += move_x_dir; 
        } 
        // If move left 
        else if ( starfi_x_next < starfi_x ) {
            // move negative direction
            starfi_x += move_x_dir; 
        }
    } 
    // if outside bounds of screen 
    else if ( (starfi_x_next - offset) <= get_X_MIN()  &&  (starfi_x_next + offset) >= get_X_MAX() ) {
        // set the move_x_dir to 0 
        set_starfi_x_dir(0); 
    }
} // end do_starfi_move
