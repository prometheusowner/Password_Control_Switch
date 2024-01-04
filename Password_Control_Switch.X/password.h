/* 
 * File:   password.h
 * Author: Barry
 *
 * Created on November 29, 2023, 5:15 PM
 */

#ifndef PASSWORD_H
#define	PASSWORD_H

#include <stdio.h>

/* Make the functions avaliable for other source files */
void restore_password();
void password_input();
int password_compare();
void password_set();
void relock();

#endif	/* PASSWORD_H */

