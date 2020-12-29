// TIME & STATES 
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
#include "screens.h"
#include "starfi.h"
#include "deflector.h"
#include "fall.h" 
#include "time.h"
// ******** CONSTANTS ************
// Teensy 
#define FREQUENCY (8000000.0)
#define PRESCALE (1.0)
#define DISTINCT_VALUES_8BIT (256.0) 
#define DISTINCT_VALUES_16BIT (65536.0) 
#define GAME_SPEED_INITIAL (1) 


// ********* GLOBALS ***************
// overflow count for time calculations / operations 
volatile uint32_t overflow_count = 0;  
// paused status initialised to true 
bool intro = true; 
bool paused = true; 
bool quit = false; 
bool stat_update = false; 
double time_game_start = 0.0; 
int game_speed = GAME_SPEED_INITIAL; 

// TEENSY 
// increment overflow count
ISR(TIMER3_OVF_vect) {
    overflow_count++; 
}

// ******** TIME *************
// SET TIME / STATE 
// set intro state 
void set_intro(bool intro_set) {
    intro = intro_set; 
}
// pause state set 
void set_paused(bool pause_set) {
    paused = pause_set; 
}
// quit state set 
void set_quit(bool quit_set) {
    quit = quit_set; 
}
// quit toggle
void set_quit_toggle(void) {
    // set opposite bool val 
    quit = !quit; 
}
// set display status 
void set_stat_update(bool update) {
    stat_update = update; 
}
// pause toggle
void set_paused_toggle(void) {
    // set opposite bool val 
    paused = !paused; 
    // also set display status to false 
    set_stat_update(false); 
} 
// set the elapsed time for the game 
void set_time_game_start(void) {
    // set start time
    time_game_start = get_elapsed_time_teensy(); 
}
// set the elapsed time for the game 
void reset_time_game_start(void) {
    // set start time
    time_game_start = 0.0; 
}
// set Game Speed
void set_game_speed(int game_speed_set) {
    game_speed = game_speed_set;  
}



// GET TIME / STATE
// returns state introduction 
bool get_intro_state(void) {
    return intro;
}
// returns paused state
bool get_game_state_paused(void) {
    return paused;
}
// returns quit state
bool get_game_state_quit(void) {
    return quit;
}
// returns status state
bool get_stat_update(void) {
    return stat_update;
}
// get elapsed time of game 
double get_time_game_start(void) {
    return time_game_start; 
}

// return Game Speed
int get_game_speed(void) {
    return game_speed;  
}

// returns elapsed time since start of teensy 
double get_elapsed_time_teensy(void) {
    // var for elapsed timer 
    double elapsed_time; 
    // Compute elapsed time  (NB:: 8 bit timer has 256 distinct ticks )
    elapsed_time = ( ( overflow_count * DISTINCT_VALUES_16BIT + TCNT3 )  // total ticks elapsed 
                        * PRESCALE / FREQUENCY ); 
    // return the ticks
    return elapsed_time; 
} 

// get elapsed time of game 
double get_elapsed_time_game(void) {
    double elapsed_game = get_elapsed_time_teensy() - get_time_game_start(); 
    return elapsed_game; 
}




// -------------------------------------------------
// Helper functions.
// -------------------------------------------------

// see rand num generator 
void seed_rand_int(void) {
    // also seed random number generator with teensy time
    srand( get_elapsed_time_teensy() );
}

// get random int
int get_rand_int(void) {
    // generate random number between 0 and 127
    int rand_num = rand();
    // return it 
    return rand_num; 
} 


// drawing helpers 
char buffer[20];

void draw_double(uint8_t x, uint8_t y, double value, colour_t colour) {
	snprintf(buffer, sizeof(buffer), "%f", value);
	draw_string(x, y, buffer, colour);
}

void draw_int(uint8_t x, uint8_t y, int value, colour_t colour) {
	snprintf(buffer, sizeof(buffer), "%d", value);
	draw_string(x, y, buffer, colour);
} 