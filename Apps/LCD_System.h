/*
 * lcd.h
 *
 *  Created on: Jan 4, 2022
 *      Author: jurado-garciaj
 */

#ifndef APPS_LCD_SYSTEM_H_
#define APPS_LCD_SYSTEM_H_

/* Set new character column value - keep existing row location
 * Range is 0 to 11
 *
 * Set new character row value - keep existing column location
// Range is 0 to 5
*/

#include "stdint.h"


//Definitions for LCD Screen
//static char WHEELS_ON[] =  "Wheels ON";
//static char WHEELS_OFF[] = "Wheels OFF";
//static  char DUTY_CYCLES[]  = "Duty:";
//static  char FREQUENCIES[]  = "[Hz]:";
//static  char DIRECTION[] = "DIRECTION:" ;
//static  char POWER[]  = "VOLT [V]:" ;
//static  char OBJECT_DETECT_NO[]  = "OBJECT: NULL" ;
//static  char OBJECT_DETECT_YES[]  = "OBJECT: YES" ;
#define ROW6       5
#define ROW5       4
#define ROW4       3
#define ROW3       2
#define ROW2       1
#define ROW1       0
#define COL11      10
#define COL10      9
#define COL9       8
#define COL8       7
#define COL7       6
#define COL6       5
#define COL5       4
#define COL4       3
#define COL3       2
#define COL2       1
#define COL1       0

/*
 * Init the LCD_SCREEN using library functions from MSOE_lib file
 * Settings: Ports configured, Clear, screen, cursor at  0,0, contrast 9
 */
 void LCD_Start();



/*
 * clears a specific row the user wants to clear
 * row options is from 0-5
 */
void LCD_Clear_ROW(unsigned char row);



/*
 * clears a specific columun the user wants to clear
 * column options is from 0-11
 */
 void LCD_Clear_COL(unsigned char col);



/*
 * sets the specific location for x_y before printing decimal values
 * row options is from 0-5 and column options is from 0-11
 *
 * void LCD_UNSIGNED_DEC_PRINT(unsigned char row, unsigned char col, unsigned short val);
 */
 void LCD_UNSIGNED_DEC_PRINT(unsigned char col, unsigned char row, unsigned short val);


 /*
  * sets the specific location for x_y before printing decimal values
  * row options is from 0-5 and column options is from 0-11
  *
  * void LCD_UNSIGNED_BIGDEC_PRINT(unsigned char row, unsigned char col, unsigned short val);
  */
 void LCD_UNSIGNED_BIGDEC_PRINT(unsigned char col, unsigned char row, float val);


/*
 *This will set the LCD Screen to the Predefined settings for motor values and power information to MSP board.
 *Setting config:
 *
 *
 *void LCD_PRESET_0();
 */
 void LCD_PRESET();

 /*
  * This will set the LCD Screen to a predefined settings for analog sensor settings
  * this is user full for the user to have a way to know each sid is being calibrated and such
  * more user friendly.
  * void LCD_CALIBRATION();
  */
void LCD_CALIBRATION();


/*
 * This will show the values of the screen when the user selects a button on the robot
 */
void LCD_CAL_value();



/*
 * This will show the values of the screen on the robot for lab exam demostration
 */
void LCD_LAB_EXAM();


#endif /* APPS_LCD_SYSTEM_H_ */
