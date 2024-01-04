/* 
 * File:   sound.h
 * Author: Barry
 *
 * Created on December 2, 2023, 6:01 PM
 */

#ifndef SOUND_H
#define	SOUND_H

/* Define the frequencies for different notes */
#define NOTE_C4 261.63  // C4 frequency in Hz
#define NOTE_D4 293.66  // D4 frequency in Hz
#define NOTE_E4 329.63  // E4 frequency in Hz
#define NOTE_F4 349.23  // F4 frequency in Hz
#define NOTE_G4 392.00  // G4 frequency in Hz
#define NOTE_A4 440.00  // A4 frequency in Hz
#define NOTE_B4 493.88  // B4 frequency in Hz

/* Make the functions avaliable for other source files */
void sound_init();
void play_music();
void play_buzz();

#endif	/* SOUND_H */

