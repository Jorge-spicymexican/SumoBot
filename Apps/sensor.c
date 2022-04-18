/*
 * Line_sensors.c
 *
 *  Created on: Jan 2, 2022
 *      Author: jurado-garciaj
 *      Objective: This file is for function calls and information for line_sensors
 *
 *  See Header file for more information
 *
 *   Rev:
 *
 *
 */

#include "Line_sensors.h"
#include "system.h"
#include "Apps/LCD_System.h"
#include "msoe_lib_lcd.h"
#include "Motor.h"

//public function prototypes
void Line_Sensor_Init(void);
void Line_Sensor_Stop(void);
void Line_Sensor_Start(void);
void Line_Sensor_Cal(void);
Direction Line_logic(void);
void Set_Cal(float left, float right);

//private function prototypes
void Power_Setup();
void ADC_Pin_Setup();
void ADC_Setup();
void Line_Sensor_Poll_Right();
void Line_Sensor_Poll_Left();
void Cal_SM();
void setting();


/*
 * Initalizes the Line Sensor for analog signals pin
 */
void Line_Sensor_Init(void){

    //set power setup pins for ADC pins
    Power_Setup();

    //set ADC's Pin setup
    ADC_Pin_Setup();

    //ADC setup
    ADC_Setup();

    ADC14->CTL0 |= ADC14_CTL0_ENC;

    return;
}


/*
 *  Sets the Power setup pins for ADC pins
 */
void Power_Setup(){

    //set pins to DIR - high
    P8->DIR |= LEFT_PWR_PIN; //P8.5
    P8->DIR |= RIGHT_PWR_PIN; //P8.6

    //set pins to OUT - high
    P8->OUT |= LEFT_PWR_PIN;
    P8->OUT |= RIGHT_PWR_PIN;

    return;
}


/*
 * Set the ADC Setup pins that Read
 */
void ADC_Pin_Setup(){

    //input pin  // '11' pin functionality
    P5->SEL0 |= LEFT_READ_PIN;
    P5->SEL0 |= RIGHT_READ_PIN;

    P5->SEL1 |= LEFT_READ_PIN;
    P5->SEL1 |= RIGHT_READ_PIN;

    P5->DIR &= ~0xFF;
    P5->DIR &= ~0xFF;


    P5->OUT &= ~0xFF;
    P5->OUT &= ~0xFF;

    //no pull up resistor
    P5->REN &= ~LEFT_READ_PIN;
    P5->REN &= ~RIGHT_READ_PIN;

    return;
}


/*
 * register ADC Setup for enabling changes
 */
void ADC_Setup(){

    /* A0 = P5.5 and A5 = P5.0
     * Keep enabled low while making changes
     *
     * CTR register is presented with a sampling rate of 32 clock cycles
     * per conversion and it is preset to one time multiple sequence.
     *
     * If you need to know how each register is configured use the register map and techincial data sheet
     * for how each bit must be configured
     */

    //resetting values
    ADC14->CTL0 = 0x00;
    ADC14->CTL1 = 0x00;
    ADC14->MCTL[0] = 0x00;
    ADC14->MCTL[2] = 0x00;

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;

    //enabled set to low
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    //clock divide -4, ADC14SC-1, pulse mode selected
    ADC14->CTL0 |= ADC14_CTL0_DIV_0 ;

    ADC14->CTL0 |= ADC14_CTL0_SHT1_5;

    ADC14->CTL0 |= ADC14_CTL0_SHP ;

    //clock - modclk, divided by 0
    ADC14->CTL0 |= ADC14_CTL0_SSEL__MODCLK ;

    ADC14->CTL0 |= ADC14_CTL0_DIV_0 ;

    //multiple signals one sequence, ADC on, ADC_MSC on
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1 ;

    ADC14->CTL0 |= ADC14_CTL0_SHT0_2 ;

    ADC14->CTL0 |= ADC14_CTL0_MSC ;

    ADC14->CTL0 |= ADC14_CTL0_ON ;

    //starting at MEM0 we will need to have ADC14CSStartADDX set to 0 if changing location of inputs
    //---Start MEM0---//
    //I am setting the resultion to 10bit because 12 bits is to much of a range for my line sensors
    ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

    //setting mem0 and mem1 for A0 and A5
    //no comparator window mode enabled
    ADC14->MCTL[0] = (ADC14_MCTLN_INCH_0) ;
    ADC14->MCTL[1] = (ADC14_MCTLN_INCH_5 | ADC14_MCTLN_EOS);

    //setting both MCTL registers to have read ADC14LO0 and ADC14LO1
    //left side = Mem[0] for ADC14LO0 register
    // ADC14->MCTL[0] &= ~ADC14_MCTLN_WINCTH;

    //right side = Mem[1] for ADC14LO1 register
    //ADC14->MCTL[1] |= ADC14_MCTLN_WINCTH;

    //enable the comparator window enabling for both memory control registers
    // ADC14->MCTL[0] |= ADC14_MCTLN_WINC;
    //ADC14->MCTL[1] |= ADC14_MCTLN_WINC;

    //set enable to high
    ADC14->CTL0 |= ADC14_CTL0_ENC;

    return;
}


/*
 * Stops the Line Sensor from sampling values
 */
void Line_Sensor_Stop(void){

    //set start to 0
    ADC14->CTL0 &= ~( ADC14_CTL0_SC );
    return;
}


/*
 * state machine for calibration setting
 */
void Cal_SM(){


    int input = TRUE;
    int change = FALSE;
    int exit = FALSE;

    while(input == TRUE){
        if( change == TRUE){
            LCD_CALIBRATION();
            change = FALSE;
        }

        if( (P1->IN & LB) ==  0 ){
            while( (P1->IN & LB) ==  0 ){
                //do this this
                while( (P1->IN & RB) ==  0 ){
                    int count = 0;
                    exit = TRUE;
                    change = TRUE;
                    LCD_clear();
                    LCD_CAL_value();
                    LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW5,right_val_black);
                    LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW4,left_val_black);
                    while(  ( ((P1->IN & RB) ==  0 ) ||  ( (P1->IN & LB) ==  0 ) ) == 1){
                        __delay_cycles(25000000); //1 seconds
                        count++;
                        LCD_UNSIGNED_DEC_PRINT(COL7,ROW6,count);
                    }
                    if( count >= 4){
                        LCD_clear();
                        LCD_goto_xy(1,2);
                        LCD_print_str("EXIT CAL");
                        input = FALSE;
                        __delay_cycles(75000000); //3 seconds

                    }
                    count = 0;

                }// end of P1->E4
            } //end while 1

            LCD_clear();
            if( exit == FALSE){
                LCD_goto_xy(1,2);
                LCD_print_str("LEFT CAL");
                __delay_cycles(50000000); //2 seconds

                Line_Sensor_Poll_Left();
                LCD_clear();
                LCD_CAL_value();
                LCD_Clear_ROW(ROW6);
                LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW4,left_val_black);
                LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW5,right_val_black);
                __delay_cycles(50000000); //2 seconds
                change = TRUE;
            } //end exit if
        }
        else{
            if( (P1->IN & RB) ==  0 ){
                while( (P1->IN & RB) ==  0 ){
                    while( (P1->IN & LB) ==  0 ){
                        int count = 0;
                        exit = TRUE;
                        change = TRUE;
                        LCD_clear();
                        LCD_CAL_value();
                        LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW5,right_val_black);
                        LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW4,left_val_black);
                        while(  ( ((P1->IN & RB) ==  0 ) ||  ( (P1->IN & LB) ==  0 ) ) == 1){
                            __delay_cycles(25000000); //1 seconds
                            count++;
                            LCD_UNSIGNED_DEC_PRINT(COL7,ROW6,count);
                        }
                        if( count >= 4){
                            LCD_clear();
                            LCD_goto_xy(1,2);
                            LCD_print_str("EXIT CAL");
                            input = FALSE;
                            __delay_cycles(75000000); //3 seconds
                        }
                        count = 0;
                    }// end of P1->E4
                }// end whole while loop
                LCD_clear();
                if( exit == FALSE){
                    LCD_goto_xy(1,2);
                    LCD_print_str("RIGHT CAL");
                    __delay_cycles(50000000); //2 seconds
                    Line_Sensor_Poll_Right();
                    LCD_clear();
                    LCD_CAL_value();
                    LCD_Clear_ROW(ROW6);
                    LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW4,left_val_black);
                    LCD_UNSIGNED_BIGDEC_PRINT(COL7,ROW5,right_val_black);
                    __delay_cycles(50000000); //2 seconds
                    change = TRUE;
                }// end of exit if statment

            }// end if
        }// end else
    } //end while

    LCD_clear();
    return;
}


/*
 * starts the Line Sensor in order to sample
 */
void Line_Sensor_Start(void){

    ADC14->CTL0 |= ADC14_CTL0_SC;

    return;
}


/*
 * calibration of Line sensors for sampling
 */
void Line_Sensor_Cal(void){

    LCD_clear();

    LCD_CALIBRATION();


    /*
     * creates an interrupt based on the conversion memory value
     * this will be used for when it reads the blaack tape since
     * this is the only thing that depending on which gets side gets triggered
     * will do the follow preset configuration to the motors
     * should make code less complex and easier to debug
     * hopefully.....
     */

    //this does not setup the ADC conversion this will have to be
    //done in main_code using the other functions

    button_init();

    Cal_SM();


    //importing the calibrated values into the low threshold registers
    cal_left = (left_val_black - .5);
    cal_right = (right_val_black - .5);

    setting();

    return;
}


/*
 *  setting for calibrated values and presenting adc conversions
 * void setting();
 */
void setting(){

    //interrupt enable for the falling short of the lower limit
    //interrupt of the window comparator for the ADC14MEMx result
    //ADC14->IER1 = ADC14_IER1_LOIE;

    //set enable to low
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;

    ADC14->CTL0 |= ADC14_CTL0_MSC ;

    ADC14->CTL0 |= ADC14_CTL0_ON ;

    //starting at MEM0 we will need to have ADC14CSStartADDX set to 0 if changing location of inputs
    //---Start MEM0---//

    ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

    //setting mem0 and mem1 for A0 and A5
    //no comparator window mode enabled
    ADC14->MCTL[0] = (ADC14_MCTLN_INCH_0) ;
    ADC14->MCTL[1] = (ADC14_MCTLN_INCH_5 | ADC14_MCTLN_EOS);

    ADC14->IER0 |= ADC14_IER0_IE0 | ADC14_IER0_IE1;

    //set enable to high
    ADC14->CTL0 |= ADC14_CTL0_ENC;

    return;
}


/*
 * function for Line Sensor Polling this is a backup or main function that will be used
 * for motor line sensor calibration used for line sensor side only
 * will try to use interrupt first and see how they play an effect with motor rotations and such
 *
 */
void Line_Sensor_Poll_Left(void){

    static int mem0;

    Line_Sensor_Start();

    while( !ADC14->IFGR0){
        ;
    }

    mem0 = ADC14->MEM[0];
    left_val_black = (3.3/2048)*mem0;
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;
    return;
}


/*
 * function for Line Sensor Polling this is a backup or main function that will be used
 * for motor controls and such
 * will try to use interrupt first and see how they play an effect with motor rotations and such
 *
 */
void Line_Sensor_Poll_Right(void){

    static int mem1;

    Line_Sensor_Start();

    while( !ADC14->IFGR0){
        ;
    }


    mem1 = ADC14->MEM[1];
    right_val_black = (3.3/2048)*mem1;

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG1;
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;

    return;
}


/*
 * interrupt handler logic for low thresholds and higher threshold values
 */
void ADC14_IRQHandler(void){

    int mem0;
    int mem1;

    /*
     * these interrupt handler should only be setting when the line sensor is below the threshold values
     * if so tell me which line sensor value is low and turn left
     * then move forward.
     */

    //printf("inside the AC14_IQR handler.\n");


    Line_Sensor_Start();

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG2;

    unsigned int intnum = ADC14->IV; //Get ADC value
    if(intnum != 0x00){
        mem0 = ADC14->MEM[0];
        left_val_black = (3.3/2048)*mem0;
        mem1 = ADC14->MEM[1]; // value from register 3
        right_val_black = (3.3/2048)*mem1;
    }

    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;
    ADC14->CLRIFGR1 |= ADC14_CLRIFGR0_CLRIFG1;

    if( ( (left_val_black < cal_left) && (right_val_black >= cal_right) ) == 1 ){

        left_black = TRUE;
        right_black = FALSE;
    }
    else if( ( (left_val_black >= cal_left) && (right_val_black < cal_right) ) == 1 ){

        left_black = FALSE;
        right_black = TRUE;
    }
    else if( ( (left_val_black < cal_left) && (right_val_black < cal_right) ) == 1 ){

        left_black = TRUE;
        right_black = TRUE;
    }
    else{

        left_black = FALSE;
        right_black = FALSE;

    }

    //setting for an interrupt that is pending
    //check if this is true

    //TODO
    /*
     * currntly memory is being overflow and so is there a timing issue
     */
    //ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLROVIFG;
    // ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRTOVIFG;


    //if(  ADC14->IV == 0x08  ){

    //clear low threshold interrupt flag
    // ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRLOIFG;

    //how do i find which interrupt flag was set my memory value??
    //this could be done by checking the

    return;

}


/*
 * line sensor logic
 */
Direction Line_logic(void){

    Direction return_val = NO_SENSOR;

    //return variable for adc value
    if((left_black == FALSE) && (right_black == FALSE)){//no line
        return_val = NO_SENSOR;
        return return_val; //no change
    }else if((left_black == FALSE) && (right_black == TRUE)){// right line
        return_val = RIGHT_SENSOR;
        return return_val;

    }else if((left_black == TRUE) && (right_black == FALSE)){//right
        return_val = LEFT_SENSOR;
        return return_val;
    }else{  //both sensors
        return_val = BOTH_SENSOR;
        return return_val;
    }
}


/*
 * void Set_Cal(void);
 *
 */
void Set_Cal(float left, float right){
    //set calibrated value
    //importing the calibrated values into the low threshold registers
    left_val_black = left;
    right_val_black = right;

    cal_left = (left_val_black - .5);
    cal_right = (right_val_black - .5);

    setting();


    return;

}
