/* 
 * File:   keypad.c
 * Author: Barry
 *
 * Created on November 29, 2023, 5:15 PM
 */

/* Include all the header files (libraries) */
#include <avr/io.h>
#include <util/delay.h>
#include "defines.h"
#include "keypad.h"

/* Initialize for Row as input and Col as output */
void row_input_col_output(void){
    /* Set ROW as the input with pull up */
    DDRD &= ~(1 << PD4 | 1 << PD3 | 1 << PD2 | 1 << PD1);
    PORTD |= (1 << PD4 | 1 << PD3 | 1 << PD2 | 1 << PD1);
    /* Set COL as the output with initial output as 0 */
    DDRB |= (1 << PB5 | 1 << PB4 | 1 << PB3 | 1 << PB1);
    PORTB &= ~(1 << PB5 | 1 << PB4 | 1 << PB3 | 1 << PB1);
}

/* Initialize for Col as input and Row as output */
void row_output_col_input(void){
    /* Set COL as the input with pull up */
    DDRD |= (1 << PD4 | 1 << PD3 | 1 << PD2 | 1 << PD1);
    PORTD &= ~(1 << PD4 | 1 << PD3 | 1 << PD2 | 1 << PD1);
    /* Set ROW as the output with initial output as 0 */
    DDRB &= ~(1 << PB5 | 1 << PB4 | 1 << PB3 | 1 << PB1);
    PORTB |= (1 << PB5 | 1 << PB4 | 1 << PB3 | 1 << PB1);
}

/* Key pad coded number structure
 * Actual Keypad:
 *  1  2  3  A
 *  4  5  6  B
 *  7  8  9  C
 *  *  0  #  D
 * Encoded Keypad:
 *  1  2  3  4
 *  5  6  7  8
 *  9 10 11 12
 * 13 14 15 16
 */

/* Function to output the keypad input */
int keypad_read(){
    int key_num = keypad_raw();
    _delay_ms(200);
    /* Function to avoid key bouncing */
    while (key_num == keypad_raw()) {
        _delay_ms(1);
    }
    return key_num; //Return the keypad coded number
}

/* Mathematical relationship for encoded keypad:
 * With the input of 1st Row key number x:
 * The key number for 1st Col is x+0;
 * The key number for 2nd Col is x+1;
 * The key number for 3rd Col is x+2;
 * The key number for 4th Col is x+3;
 */

/* Function to read the keypad input */
int keypad_raw(){
    /* Initialize the keypad input as coded void */
    int key = 0;
    
    /* Function to determine the Row number of the key been pressed */
    row_input_col_output();
    while (!((PIND & (1 << PD4))||(PIND & (1 << PD3))||(PIND & (1 << PD2))||(PIND & (1 << PD1)))){} // Wait till there is an input signal
    _delay_ms(20);  //Delay to avoid key bouncing
    if (!(PIND & (1 << PD4))){key = 1;}
    else if (!(PIND & (1 << PD3))){key = 5;}
    else if (!(PIND & (1 << PD2))){key = 9;}
    else if (!(PIND & (1 << PD1))){key = 13;}
    
    /* Function to determine the Col number of the key been pressed */
    row_output_col_input();
    while (!((PINB & (1 << PB4))||(PINB & (1 << PB3))||(PINB & (1 << PB5))||(PINB & (1 << PB1)))){} // Wait till there is an input signal
    _delay_ms(20);  //Delay to avoid key bouncing
    if (!(PINB & (1 << PB4))){key = key + 1;}
    else if (!(PINB & (1 << PB3))){key = key + 2;}
    else if (!(PINB & (1 << PB1))){key = key + 3;}
    _delay_ms(10);
    return key;
}
