/* 
 * File:   password.c
 * Author: Barry
 *
 * Created on November 29, 2023, 5:15 PM
 */

/* Include all the header files (libraries) */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "defines.h"
#include "password.h"
#include "sound.h"
#include "led.h"
#include "keypad.h"

/* Define strings for print */
char LOCK[] = "LOCK           ";
char UNLOCK[] = "UNLOCK           ";
char RESET[] = "RESET           ";
char NUMBER[] = "NUMBER:";
char INPUT_CLEAN[] = "INPUT:           ";
char INPUT[] = "INPUT:";
char WRONG[] = "WRONG           ";

/* Initial the int */
static int compareflag = 0;
static int input_num = 0;

/* Array to store the input password */
char input_password[6] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

/* Array to store the set password */
char set_password[6] = {
    {1},
    {2},
    {3},
    {5},
    {6},
    {7},
};

/* Restore the set password from EEPROM */
void restore_password(){
    for (int a=0; a<6; a++){
        set_password[a] = eeprom_read_byte((uint8_t *) a);  // Store the value from EEPROM to the array
    }
}

/* Function to ask the user to input the password */
void password_input() {
    fprintf (stderr, "\x1b\x80%s", LOCK);   // Showing the state is lock
    fprintf (stderr, "\x1b\xC0%s", INPUT_CLEAN);    // Clear the message from previous
    fprintf (stderr, "\x1b\xC0%s", INPUT);  // Showing the input key
    input_num = 0;  // Reset the input number back to 0 for the next input
    /* Repeat for 6 times for the input password */
    for (int j=0; j < 6; j++) {
        /* Make sure the input key is not void or D */
        while ((input_num == 0) || (input_num == 16)){
            input_num = keypad_read();
        }
        input_password[j] = input_num;  //Store the input key into the array
        print_num(input_num);   //Print the input key on LCD
        input_num=0;    // Reset the input number back to 0 for the next input
    }
}

/* Compare the input password to the set password and return the compare result*/
int password_compare() {
    /* Repeat 6 times to the check the values from two arrays one by one */
    for (int i=0; i<6; i++){
        if (input_password[i] == set_password[i]){
            compareflag = 1;    // Set the compare flag to 1 when the number is the same
        }
        else {
            compareflag = 0;    // Set the compare flag to 0 when the number is not the same
            break;  //Break the for loop
        }
    }
    
    /* Based on the compare result to choose the message to print */
    if (compareflag) {
        fprintf (stderr, "\x1b\x80%s", UNLOCK); // Showing the system is unlocked now
        fprintf (stderr, "\x1b\xC0%s", UNLOCK);
        system_on();    // Turn on the system
        play_music();   // Play the music
        return 1;
    }
    else {
        fprintf (stderr, "\x1b\x80%s", LOCK);   // Showing the system is still locked
        fprintf (stderr, "\x1b\xC0%s", WRONG);  // Showing the input password is wrong
        play_buzz();    // Play the beep sound
        return 0;
    }
}

/* Function to set the password to the most recently 6 inputs */
void password_set() {
    fprintf (stderr, "\x1b\x80%s", RESET);  // Showing the state is password reset
    fprintf (stderr, "\x1b\xC0%s", NUMBER); // Showing the reset password
    /* Repeat for 6 times for to set the password one by one */
    for (int k=0; k<6; k++) {
        int num = input_password[k];
        print_num(num); // Print the password been set now
        set_password[k] = num;
        eeprom_write_byte((uint8_t *)k, num);   // Store the value from the array to EEPROM
    }
}

/* Print the key based on the encoded number (Encoded method is showing in keypad.c) */
void print_num(int num) {
    switch (num) {
        case 13:
            fprintf (stderr, "*");
            break;
        case 14:
            fprintf (stderr, "0");
            break;
        case 15:
            fprintf (stderr, "#");
            break;
        case 4:
            fprintf (stderr, "A");
            break;
        case 8:
            fprintf (stderr, "B");
            break;
        case 12:
            fprintf (stderr, "C");
            break;
        case 16:
            fprintf (stderr, "D");
            break;
        case 5:
            fprintf (stderr, "4");
            break;
        case 6:
            fprintf (stderr, "5");
            break;
        case 7:
            fprintf (stderr, "6");
            break;
        case 9:
            fprintf (stderr, "7");
            break;
        case 10:
            fprintf (stderr, "8");
            break;
        case 11:
            fprintf (stderr, "9");
            break;
        default:
            fprintf (stderr, "%1d", num);
            break;
    }
    return 0;
}

/* Function to lock the system again */
void relock(){
    fprintf (stderr, "\x1b\x80%s", UNLOCK);
    fprintf (stderr, "\x1b\xC0%s", UNLOCK);
    input_num = 0;
    /* Wait till the D key is been pressed */
    while ((input_num != 16)){
            input_num = keypad_read();
        }
    system_off();   // Turn the system
}