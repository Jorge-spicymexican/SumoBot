/*
 * Timer32.h
 *
  *  Created on: Jan 12, 2022
 *      Author: jurado-garciaj
 *      Objective: This timer to toggle a pin at 38KHz wave with 50% duty cycle
 *
 *      Rev:
 *
 *
 */

#ifndef APPS_TIMER32_H_
#define APPS_TIMER32_H_

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"


//function prototypes
void Timer32_Init(void);
void Timer32_Start(void);
void Timer32_Stop(void);
int Timer32_getCount(void);

//Global variables for IQR
int lol;


/*
 * Timer32-Init
 *
 * initalizes the timer registers to be enabled in free running
 * using setting frequency at 38KHz for timing and also interrupts
 *
 */


/*
 * Timeer32_Start
 *
 * enable timer count to start counting
 *
 */


/*
 * Timer32_Stop
 *
 * stop the timer from counter and reset timer
 *
 */


/*
 * Timer32_getCount
 *
 * Get the count value of the timer32
 *
 */


#endif /* APPS_TIMER32_H_ */
