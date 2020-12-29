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
// include teensy header 
#include "setup_teensy.h"
#include "time.h"
#include "starfi.h"
#include "fall.h"
#include "deflector.h" 
#include "screens.h" 

/* 
// TO BE IMPLIMENTED
// USB
void setup_usb (void) {
    //serial_USB();
    // adc 
    // adc_init();
}
*/ 

void setup_teensy_inputs(void) {
    // Enable Left, Right, Up, and Down joystick switches
    CLEAR_BIT(DDRB, 1);     // JS LEFT input (set input - clear = 0) 
    CLEAR_BIT(DDRD, 0);     // JS RIGHT input 
    CLEAR_BIT(DDRD, 1);     // JS UP
    CLEAR_BIT(DDRB, 7);     // JS DOWN 
    CLEAR_BIT(DDRB, 0);     // JS CENTRE
    // Enable input from the left and right buttons
    CLEAR_BIT(DDRF, 6); // LEFT
    CLEAR_BIT(DDRF, 5); // RIGHT
}

// draw intro screen 
void draw_teensy_intro_screen(void) {
    // y-offset for animation, initially 0
    int y_offset = 0; 
    y_offset = ( ( (int)get_elapsed_time_teensy() * 2) % 10) - 5; 
    // student number
    draw_string(17, 2, "ASTRO !!!", FG_COLOUR); 
    // draw animation
    draw_string(0, (18 - y_offset), "* * * * * * * * *", FG_COLOUR); 
    draw_string(0, (23 + y_offset), " * * * * * * * *", FG_COLOUR); 
    // student number
    draw_string(18, 40, "n10348328", FG_COLOUR); 
}

// setup normal overflow for teensy 
void setup_overflow_normal(void) {
    // Timer 3 (TCCR3) in normal mode, 
    TCCR3A = 0; // zero for normal mode 
    TCCR3B = 1;  // with value 1 = pre-scaler 1 ==> ~8MHz overflow.
    // turn overflow on 
    TIMSK3 = 1;  // with value 1 = overflow on 
    // Turn on interrupts globally.
    sei(); 
} // end setup overflow normal 


// Setup the initial screen 
void setup_teensy_initial(void) {
    // set into state 
    set_intro(true); 
    // clock speed for teensy 
    set_clock_speed(CPU_8MHz); 
    // initialise lcd to default contrast 
    lcd_init(LCD_DEFAULT_CONTRAST);
    // start timer / overflow count
    setup_overflow_normal(); 
    // Set key inputs 
    setup_teensy_inputs(); 
}
