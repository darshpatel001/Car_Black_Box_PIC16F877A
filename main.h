/* 
 * File:   main.h
 * Author: darsh
 *
 * Created on May 27, 2026, 9:40 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "EEprom.h"
#include "timers.h"
#include "uart.h"
#include <string.h>


#define DASHBOARD_SCREEN        0x00
#define LOGIN_SCREEN            0x01
#define MAIN_MENU_SCREEN        0x02
#define VIEW_LOG_SCREEN         0x03
#define SET_TIME_SCREEN         0x04
#define CHANGE_PASSWORD_SCREEN  0x05
#define DOWNLOAD_LOG_SCREEN     0x06
#define RESET_LOGIN_SCREEN      0x07
#define CLEAR_LOG_SCREEN        0x08
#define RETURN_MAIN_MENU        0x09

#define RESET_MENU              0x0A
#define RESET_VIEW_LOG          0x0B
#define RESET_NOTHING           0x0C
#define RESET_CHANGE_PASSWORD   0x0D
#define RESET_CLEAR_LOG         0x0E
#define RESET_DOWNLOAD_LOG      0xF0
#define RESET_SET_TIME          0x10

#define CHANGE_PASSWORD         0x10

#define PASSWORD_CHANGED        0x11

#define RETURN_BACK             0x12
#define LOGIN_SUCCESS           0x13

#define VIEW_LOG                0xAA
#define CLEAR_LOG               0xBB
#define DOWNLOAD_LOG            0xCC
#define SET_TIME                0xDD


#define LONG_PRESS              25


#endif	/* MAIN_H */

