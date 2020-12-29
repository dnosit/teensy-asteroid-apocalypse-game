// ********* SCREEN VERSIONS & INFO ************
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
#include "deflector.h"
#include "fall.h"
// centre of screen (84w x 48h  pixels)
#define X_MAX (83)
#define X_MIN (0)
#define Y_MAX (47)

// ******* FUNCTIONS *************
// ******** GET *********
// return X_MAX
int get_X_MAX(void){
    return X_MAX; 
}
// return X_MIN
int get_X_MIN(void){
    return X_MIN; 
}
// return Y_MAX
int get_Y_MAX(void) {
    return Y_MAX; 
}

// ******** SET *********

// ********  SCREENS  **********
// display the game stats 
void draw_game_status(void){ 
    // i. Game time
    draw_string(0, 1, "Game time:", FG_COLOUR);
    draw_double(10,9, get_elapsed_time_game(), FG_COLOUR); 
    // ii. Remaining Useful Life
    draw_string(0, 17, "Lives remain:", FG_COLOUR);
    draw_int(10, 25, get_lives(), FG_COLOUR); 
    // iii. Score
    draw_string(0, 33, "Score:", FG_COLOUR);
    draw_int(10, 41, get_starfi_score(), FG_COLOUR); 
}

// do game status update
void do_game_status_update(void) {
    // set state to update 
    set_stat_update(true); 
    // send info to computer 

} // end game status update

// draw the game screen 
void draw_game_screen(void) {
    // draw deflector sheild at row 39 if lives remaining
    draw_deflector(); 
    // draw spaceship 
    draw_starfi(); 
    // draw asteroids 
    draw_asteroids_all();
    // draw boulders 
    draw_boulders_all();
    // draw fragments 
    draw_fragments_all(); 
    // draw plasma 
    draw_plasma_all(); 
} // and draw game screen 

// draw quit screen 
void draw_quit_screen(void) {
    // change screen to inverse mode 
    LCD_CMD(lcd_set_display_mode, lcd_display_inverse);
    // student number
    draw_string(18, 40, "n10348328", FG_COLOUR); 
} 

// reset game 
void do_game_reset (void) {
    // seed random number gen  
    seed_rand_int(); 
    // set normal video mode 
	LCD_CMD(lcd_set_display_mode, lcd_display_normal);
    // stop the intro / quit screen 
    set_intro(false);
    // quit state false
    set_quit(false); 
    // set initial state to paused 
    set_paused(true); 
    // set stat update false
    set_stat_update(false);
    // reset game time start to 0.0
    reset_time_game_start(); 
    // set game spoeed 
    set_game_speed(1);
    // reset starfighter params 
    do_starfi_reset(); 
    // clear falling objects 
    clear_falling_objects(); 
    // reset plasma 
    clear_plasma(); 
    // check & respawn at start
    respawn_if_required();
    // reset deflector lives 
    reset_lives(); 
} // end game reset 


// IF NOT PAUSED & game started - update game parameters 
void do_game_update(void) {
    // save current time as int 
    int game_time = (int)get_elapsed_time_game(); 
    // if game time elapsed not yet set, set it 
    if ( get_time_game_start() == 0.0 ) {
        // Set game start time to current elapsed teensy time 
        set_time_game_start(); 
    }
    // if less than X seconds game time, show message "game started" 
    if ( game_time <= 2 ) { 
        // game started message
        draw_string(10, 20, "Game Started!", FG_COLOUR); 
    }
    // asteroids start after 2 seconds 
    if ( game_time >= 2 ) { 
       // update asteroids 1 step of game speed 
        do_move_asteroid( get_game_speed() ); 
    }
    // game resets with random direction based on time mod even/odd
    // so, then update starfighter randomly left or right
    do_starfi_move( get_starfi_x_dir() ); 
        // NB: reset to 0 when when button pressed or at wall 
    // update boulder 
    do_move_boulder( get_game_speed() ); 
    // update fragments 
    do_move_fragment( get_game_speed() ); 
    // update plasma
    do_move_plasma( get_game_speed() ); 
} // end do game updates 
