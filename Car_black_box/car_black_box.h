
/*
 * File:   car_black_box.h
 * Author: ADMIN
 *
 * Created on 20 May, 2025, 5:20 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

#include "main.h"

void display_dashboard(unsigned char event[] , unsigned char speed);
static void get_time();
void display_time();
void log_event(unsigned char event[], unsigned char speed);
void clear_display();
unsigned char login_screen(unsigned char key, unsigned char reset_flag);
unsigned char menu_screen(unsigned char key, unsigned char reset_flag);

// Function prototypes (declare all used functions)
void clear_display();

void view_log();
void clear_log();
void download_log();
void change_passwd();
void set_time();

#endif	/* CAR_BLACK_BOX_H */


