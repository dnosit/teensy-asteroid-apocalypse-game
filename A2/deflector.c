// ********* DEFLECTOR ************
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
#include <macros.h>    // <.h> for other directory 
#include "lcd_model.h" // ".h" for same directory header
// ********** Other teensy modules in this folder 
// #include "usb_serial.h"
// #include "cab202_adc.h" 
// ********** A2 other modules in this folder
#include "setup_teensy.h" 
#include "time.h" 
#include "starfi.h"
#include "screens.h"
#include "fall.h"
// **** DEFINE ****
#define DEFLECTOR_Y (39)
#define INITIAL_LIVES (5)
// **** GLOBAL ******
int lives = INITIAL_LIVES; 

// ******* FUNCTIONS *********
// return deflector y
int get_DEFLECTOR_Y(void) {
    return DEFLECTOR_Y; 
}

// returns lives 
int get_lives(void) {
    return lives; 
}

// reset lives 
void reset_lives(void) {
    lives = INITIAL_LIVES; 
}

// reduce lives by amount
void do_reduce_life(int amount) {
    // predicted life 
    int life_next = lives - amount;
    // if amount not less than 0, reduce 
    if (life_next >= 0) {
        // reduce by amount
        lives -= amount; 
    }
    else { // lives = 0
        lives = 0; 
    }
} // end reduce life 


// reset deflector 
void do_deflector_reset(void) {
    // reset lives 
    lives = INITIAL_LIVES; 
}

// draw deflector 
void draw_deflector(void) {
    // draw deflector sheild at row 39 
    for (int x = 0; x < get_X_MAX(); x+=4) {
        // draw 2 pixel line every 4 pixels
        draw_line(x, DEFLECTOR_Y, x + 2, DEFLECTOR_Y, FG_COLOUR);
    } // end for deflector shield 
}

// ************** COLLISIONS **************

// retruns true if collision with deflector 
bool deflector_collision(int obj_y_bottom ) {
    if (obj_y_bottom < get_DEFLECTOR_Y() ) {
        // return false
        return false; 
    }
    else {
        // true
        return true; 
    }
} // end deflector_collide check 

// do game over actions 
void do_game_over(void) {
    // game over message
    draw_string(10, 20, "Game Over!!!", FG_COLOUR); 
}

// return true if game over & display message
bool game_over(void) {
    // check if game over 
    if (lives == 0) return true;  
    else return false; 
} 