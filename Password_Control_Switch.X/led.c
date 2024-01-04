/* 
 * File:   led.c
 * Author: Barry
 *
 * Created on November 29, 2023, 5:15 PM
 */

/* Include all the header files (libraries) */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "led.h"

/* Initialize the LED port for output */
void led_init(){
    DDRD |= (1 << PD0); // Make PD0 an output.
    DDRD |= (1 << PD6); // Make PD6 an output.
}
 /* Turn on the system */
void system_on(){
    PORTD &=~(1 << PD0); // Turn off PD0 (Red RGB).
    _delay_ms (1);
    PORTD |= (1 << PD6); // Turn on PD6 (Green RGB).
    return 0;
}

/* Turn off the system */
void system_off(){
    PORTD &=~(1 << PD6); // Turn off PD6.
    _delay_ms (1);
    PORTD |= (1 << PD0); // Turn on PD0.
    return 0;
}