/* 
 * File:   sound.c
 * Author: Barry
 *
 * Created on November 29, 2023, 5:15 PM
 */

/* Include all the header files (libraries) */
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "defines.h"
#include "sound.h"

/* Initial the output for Buzzer */
void sound_init(){
    /* Make PB2 as output pin with pull-up */
    PORTB &= (1 << PORTB2 );
    DDRB |= (1 << DDB2 );
}

/* Function to play a certain frequency */
void play_note(float fre) {
    /* Start timmer 1 with no output */
    ICR1 = (F_CPU/(fre) - 1);   // ICR1=Clock Frequency/(Frequency * Prescaler)-1
    /* Clear OC1B on compare match. */
    TCCR1A = (1 << WGM11) | (0 << WGM10) | (0 << COM1B1 ) | (0 << COM1B0 );
    /* Normal mode, no prescaler. */
    TCCR1B = (1 << WGM13) | (0 << WGM12) | (1 << CS10);
    TIFR1 |= (1 << OCF1B); // Clear the flag
        
    /* Start the Buzzer PWM wave */
    for (int i=0; i<50; i++) {
        TCNT1 = 0;
        TCCR1A = (1 << WGM11) | (0 << WGM10) | (1 << COM1B1) | (1 << COM1B0);   // Toggle OC1B output on Compare Match (waveform generation)
        while ((TIFR1 & (1 << OCF1B))!=(1 << OCF1B)){}  // Wait till output compare B match flag is set
        TIFR1 |= (1 << OCF1B); // Clear the flag
        TCCR1A = (1 << WGM11) | (0 << WGM10) | (1 << COM1B1) | (0 << COM1B0);   // Toggle OC1B off to generate the remaining portion of the cycle (50%)
        while ((TIFR1 & (1 << OCF1B))!=(1 << OCF1B)){}  // Wait for the current cycle to complete
        TIFR1 |= (1 << OCF1B); // Clear the flag
        _delay_ms(1);
    }
        
    /*Reset the timmer 1 to no output*/
    TCCR1A = (1 << WGM11) | (0 << WGM10) | (0 << COM1B1 ) | (0 << COM1B0 ); // Clear OC1B on compare match
    TIFR1 |= (1 << OCF1B); // Clear the flag
}

/* Function to play the music when password is right */
void play_music(){
    play_note(NOTE_C4);
    play_note(NOTE_D4);
    play_note(NOTE_E4);
    play_note(NOTE_F4);
    play_note(NOTE_G4);
    play_note(NOTE_A4);
    play_note(NOTE_B4);
    play_note(NOTE_C4);
    play_note(NOTE_D4);
    play_note(NOTE_E4);
}

/* Function to play the beep sound when password is wrong */
void play_buzz(){
    for (int j=0; j<5; j++){
        play_note(NOTE_C4);
        _delay_ms(500); //Delay for 500ms to create the beep sound
    }
}
