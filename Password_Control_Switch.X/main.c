/* 
 * File:   main.c
 * Author: Barry
 *
 * Created on November 25, 2023, 3:29 PM
 */

/* Include all the header files (libraries) */
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "password.h"
#include "defines.h"
#include "lcd.h"
#include "keypad.h"
#include "led.h"
#include "sound.h"

/* Set up LCD FILE structure for writing */
static FILE lcd = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

/* Set the compare flag to store the information about if the input password is right */
int compareFlag = 0;

/* Initialize the interrupt (key interrupt for PB7 (on board interrupt)) */
void interrupt_init(){
    DDRB &= ~(1 << PB7);    // Set PB7 as input
    
    PCICR |= (1 << PCIE0);
    PCIFR |= (1 << PCIF0);
    PCMSK0 |= (1 << PCINT7);
    sei();  // Initial global intrrupt
}

/* Create the interrupt function */
ISR(PCINT0_vect, ISR_BLOCK) {
    password_set(); // Reset the password when interrupt occurs
}

/* Initialization */
void init(void){
    interrupt_init();
    lcd_init();
    led_init();
    sound_init();
}

/* Main function */
int main(void) {
    
    /* Initialize everything that is needed */
    init();
    
    /*Set the stderr for lcd write*/
    stderr = &lcd;
    
    /* Restore the password from EEPROM when start up */
    restore_password();
    
    /* while loop for the repeating tasks */
    while(1){
        /* Turn off the system */
        system_off();
        
        compareFlag = 0;    // Set the compare flag to 0
        
        /* Keep asking for password if the input password is not correct */
        while (compareFlag == 0){
            password_input();   // Ask for the input password
            compareFlag = password_compare();   // Check if the input password matches
        }
        relock();   // Wait till the user lock the system
    }
    
    return 0;
}

