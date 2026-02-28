
/*
 * File:   main.h
 * Author: ADMIN
 *
 * Created on 20 May, 2025, 5:17 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include<xc.h>
#include "adc.h"
#include "clcd.h"
#include "i2c.h"
#include "uart.h"
#include "ds1307_.h"
#include "car_black_box.h"
#include "digital_keypad.h"
#include <string.h>
#include"Eeprom.h"
#include"timers.h"


#define DASHBOARD_SCREEN 0x00
#define LOGIN_SCREEN 0x01
#define MAIN_MENU_SCREEN 0x02

#define RESET_PASSWORD 0x11
#define RESET_NOTHING 0x22
#define RETURN_BACK 0x33
#define LOGIN_SUCCESS 0x44
#define RESET_MENU 0x55

#define VIEW_LOG_SCREEN        0x10
#define CLEAR_LOG_SCREEN       0x20
#define DOWNLOAD_LOG_SCREEN    0x30
#define CHANGE_PASSWORD_SCREEN 0x40
#define SET_TIME_SCREEN        0x50

#define MAX_LOGS 4
#define LINE1(x) (0x80 + (x))
#define LINE2(x) (0xC0 + (x))
#define LOG_LENGTH 10



#define LONG_PRESS_THRESHOLD 50  // 50 x 200ms = 10s long press

#define POLL_DELAY 10         // ms polling delay



#endif	/* MAIN_H */



