/*
 * system.h
 *
 *  Created on: Dec 18, 2021
 *      Author: jurado-garciaj
 *      Objective: This file creates definitions and type def structs for
 *      pins being used on the MSP board. Should help us de-code beter and keep
 *      the main code cleaner.
 *
 *      Rev:
 *
 */

#ifndef APPS_SYSTEM_H_
#define APPS_SYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <msp.h>


/*
 * Definitions for data types and definitions
 *
 */

#define TRUE            1
#define FALSE           0


#define RED    BIT0
#define BLUE   BIT2
#define GREEN  BIT1
#define ALL    0xFF


#define forward_value 200
#define turn_value 150
#define _1_sec __delay_cycles(4*2500000) //1 second
#define _2_sec  __delay_cycles(4*5000000) //2 seconds
#define _5_sec  __delay_cycles(4*12500000) //5 seconds
#define _4_sec  __delay_cycles(4*10000000) //4 seocnds
#define _0_5_sec  __delay_cycles(4*1750000) //0.5 seconds
#define _0_25_sec __delay_cycles(4*97500) //0.25 seconds
#define _0_10_sec _delay_cycles(4*69500)
#define turn_rev __delay_cycles(250000)


//function prototypes
//resets all of the pins on the MSP Board when having to debug or restart program
//and blinks a Green Led
void SystemReset();


//turns on all of the MSP432 Pins on and blinks Red LED
void SystemOn();

//rounding function for my system
int rounding(float num);

//blinking function for code
void blink(int Color);

void button_init(void);

#endif /* APPS_SYSTEM_H_ */
