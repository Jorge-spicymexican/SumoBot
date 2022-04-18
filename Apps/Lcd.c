/*
 * Lcd.c
 *
 *  Created on: Jan 4, 2022
 *      Author: jurado-garciaj
 */


/*
 * Include all msoe library files
 */
#include <Apps/LCD_System.h>
#include <stdio.h>
#include <float.h>
#include "msp432.h"
#include "stdint.h"
#include "msoe_lib_lcd.h"


//public function prototypes
void LCD_Start();
void LCD_Clear_ROW(unsigned char row);
void LCD_Clear_COL(unsigned char col);
void LCD_UNSIGNED_DEC_PRINT(unsigned char row, unsigned char col, unsigned short val);
void LCD_UNSIGNED_BIGDEC_PRINT(unsigned char col, unsigned char row,  float val);
void LCD_PRESET();
void LCD_CALIBRATION();
void LCD_CAL_value();
void LCD_LAB_EXAM();


//Code implementation//
/*
 * Starts the LCD Screen configurations, clears lcs, Set x_y at 0,0
 * and contrast is at the most high
 */
void LCD_Start(){
    LCD_Config();
    LCD_clear();
    LCD_home();
    LCD_contrast(10);

    return;
}


/* go to row location row == y location
 *  Cycle through each 8-bit column location and write all blanks
 *  1 banks x 84 columns = 84 8-bit column locations
 *  The LCD controller automatically handles the wrap-around at the end of a row
 */
void LCD_Clear_ROW(unsigned char row){
    LCD_goto_xy(0,row);
    unsigned short i;
    for(i=0; i<84; i++)
        LCD_Data_WR(0x00);
    return;
}


/* go to row location row == y location
 *  Cycle through each 8-bit column location and write all blanks
 *  1 banks x 84 columns = 84 8-bit column locations
 *  The LCD controller automatically handles the wrap-around at the end of a row
 */
void LCD_Clear_COL(unsigned char col){
    LCD_goto_xy(col,0);
    unsigned char i;
    unsigned char row_val;

    //for loop one it clears that specific bit in its row
    //go to the next row_val column
    for(row_val=0; row_val<6; row_val++){
        LCD_goto_xy(col,row_val);
        for(i=0;i<7; i++){   //For is 84 8-bit column location therefore each pixel is a 42 bit
            LCD_Data_WR(0x00);
        }
    }
    return;
}


/*
 * set the X and Y location for pixel useful for less complicated code and prints unsigned decimal value
 */
void LCD_UNSIGNED_DEC_PRINT(unsigned char col, unsigned char row, unsigned short val){

    LCD_goto_xy(col, row);

    LCD_print_udec3(val);
    return;
}


/*
 * set the X and Y location for pixel useful for less complicated code and prints unsigned decimal value
 */
void LCD_UNSIGNED_BIGDEC_PRINT(unsigned char col, unsigned char row, float val){

    LCD_goto_xy(col, row);

    LCD_print_float(val);
    return;
}


/*
 * Preset values for information:
 *
 *
 *      | C1 | C2 | C3 | C4 | C5 | C6 | C7 | C8 | C9 | C10 | C11 | C12 |
 * | R1 |             WHEEL INFO                                       |
 * | R2 |              POWER                                           |
 * | R3 |            DIRECTION                                         |
 * | R4 |               DUTY                                           |
 * | R5 |            FREQUENCY                                         |
 * | R6 |           OBJECT INFO                                        |
 * |_XX_|______________________________________________________________|
 *
 *
 */
void LCD_PRESET(){

    //go to R1 & C1 and printf wheel info
    LCD_goto_xy(COL2,ROW1);
    LCD_print_str("Wheels OFF");

    //printf POWER
    //LCD_goto_xy(COL1,ROW2);
    //LCD_print_str(POWER);

    //printf Direction
    LCD_goto_xy(COL1,ROW2);
    LCD_print_str("DIRECTION:");

    //printf Duty
    LCD_goto_xy(COL1,ROW3);
    LCD_print_str("Duty:");

    //printf Freq.
    LCD_goto_xy(COL1,ROW4);
    LCD_print_str("[Hz]:");
    //LCD_print_bigstr(ROW5, COL1, FREQUENCIES);

    //printf object
    LCD_goto_xy(COL1,ROW6);
    LCD_print_str("OBJECT: NO");
    //LCD_print_bigstr(ROW6, COL1, OBJECT_DETECT);

    //reset x and y corrdinates to 0,0
    LCD_home();

    return;

}


/*
 * Preset values for Calibration information:
 *
 *
 *      | C1 | C2 | C3 | C4 | C5 | C6 | C7 | C8 | C9 | C10 | C11 | C12 |
 * | R1 |             Calibration                                      |
 * | R2 |             LINESENSOR                                       |
 * | R3 |           Press LB for Left Side                             |
 * | R4 |           Press RB for Left Side                             |
 * | R5 |            LS CAL Value                                      |
 * | R6 |           OBJECT INFO                                        |
 * |_XX_|______________________________________________________________|
 *
 *
 */
void LCD_CALIBRATION(){
    //go to R1 & C1 and printf calibration
    LCD_goto_xy(COL1,ROW1);
    LCD_print_str("Calibration");

    //go to R1 & C1 and printf calibration
    LCD_goto_xy(COL1,ROW2);
    LCD_print_str("Line Sensor");

    //Press right side button
    LCD_goto_xy(COL1,ROW4);
    LCD_print_str("LB for Left ");

    //Press left side button
    LCD_goto_xy(COL1,ROW5);
    LCD_print_str("RB for Right ");

    LCD_goto_xy(COL1,ROW6);
    LCD_print_str("LB & RB-Exit");
}

void LCD_CAL_value(){
    //go to R1 & C1 and printf calibration
    LCD_goto_xy(COL1,ROW1);
    LCD_print_str("Calibration");

    //go to R1 & C1 and printf calibration
    LCD_goto_xy(COL1,ROW2);
    LCD_print_str("Line Sensor");

    //Press right side button
    LCD_goto_xy(COL1,ROW4);
    LCD_print_str("L Val:");

    //Press left side button
    LCD_goto_xy(COL1,ROW5);
    LCD_print_str("R Val:");

    LCD_goto_xy(COL1,ROW6);
    LCD_print_str("Count:");
}


void LCD_LAB_EXAM(){

    //go to R1 & C1 and printf Exam 1
    LCD_goto_xy(COL1,ROW1);
    LCD_print_str("Exam 1");

    //go to R2 & C4 and printf EE2931
    LCD_goto_xy(COL4,ROW2);
    LCD_print_str("EE2931");

    //go to R3 & C1 and printf - 011
    LCD_goto_xy(COL6,ROW3);
    LCD_print_str("- 011");

    //go to R6 & C9 and printf STEP
    LCD_goto_xy(COL9,ROW6);
    LCD_print_str("STEP");



}
