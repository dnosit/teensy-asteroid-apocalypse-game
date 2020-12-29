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
#include "screens.h" 
#include "starfi.h" 
#include "deflector.h" 
#include "fall.h"
// ********* GLOBALS ************


// ******** Main Processing ********* 

// process input commands 
void process_inputs(void) {
    // ACTIONS BY INPUT: 
    // LEFT BUTTON or keyboard 'r' - quit intro / reset game
	if ( BIT_IS_SET(PINF, 6) ) { 
        // start / reset the game 
        do_game_reset(); 
	} 
    // RIGHT BUTTON or keyboard 'q' - quit intro / reset game
    else if ( BIT_IS_SET(PINF, 5) ) {
        // quit state true 
        set_quit(true); 
    }
    // UP JOYSTICK or keyboard 'w'
    else if ( BIT_IS_SET(PIND, 1) ) {
        do_plasma(); 
    }
	// DOWN JOYSTICK or 's' - display game status 
    else if ( BIT_IS_SET(PINB, 7) ) { 
        // update status screen / computer 
        do_game_status_update(); 
    }
	// LEFT JOYSTICK of keyboard 'a' - move starfighter_x left 
    else if ( BIT_IS_SET(PINB, 1) ) {
        // move starfighter left 
        do_starfi_move(-1); 
        // stop automove 
        set_starfi_x_dir(0); 
    }
	// RIGHT JOYSTICK or keyboard 'd'- move starfighter_x right 
    else if ( BIT_IS_SET(PIND, 0) ) {
        // move stargihter right 
        do_starfi_move(1); 
        // stop automove 
        set_starfi_x_dir(0); 
    }
    // CENTRE JOYSTICK or 'p' - pause / resume game 
    if ( BIT_IS_SET(PINB, 0)  ) {
        // toggle the pause 
        set_paused_toggle(); 
    } 
	// if no switches closed, do nothing.
    else { ; } // empty else 
}

// main processing 
void process( void ) {
    // check if new instructions 
    process_inputs(); 
    // Clear screen
    clear_screen(); 
    // draw: 
    // game screen states
    if ( !get_game_state_quit() && !get_intro_state() ) {
        // Do game over action if game over, on top of game screen
        if ( game_over() ) {
            draw_game_screen(); 
            do_game_over(); 
        }
        // if game NOT paused, update parameters 
        if ( !get_game_state_paused() && !game_over() )  {
            // update game parameters 
            do_game_update(); 
            // draw game screen
            draw_game_screen(); 
        }
        // if paused & display status
        else if ( get_game_state_paused() && get_stat_update() ) {
            // draw status 
            draw_game_status(); 
        }
        // if paused & display status
        else if ( get_game_state_paused() && !get_stat_update() ) {
            // draw game screen WITHOUT updating the game 
            draw_game_screen(); 
        }
    }
    // intro screen state
    else if ( get_intro_state() ) {
        // draw intro
        draw_teensy_intro_screen();
    } 
    // quit screen state 
    else if ( get_game_state_quit() ) {
        // draw quit screen 
        draw_quit_screen();
    } 
    // Show screen
    show_screen(); 
} 

// Main 
int main(void) {
    // set initial state 
    setup_teensy_initial();
    // main loop 
	for ( ;; ) {
        // main process 
		process();
        // delay 
		_delay_ms(100);
	}
	return 0;
}

