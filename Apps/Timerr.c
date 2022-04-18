/*
 * _Timer32.h
 *
 *  Created on: Jan 12, 2022
 *      Author: jurado-garciaj
 *      Objective: This timer to toggle a pin at 38KHz wave with 50% duty cycle
 *
 *      Rev:
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "Apps/Timer32.h"

//public function prototypes
void Timer32_Init(void);
void Timer32_Start(void);
void Timer32_Stop(void);
int Timer32_getCount(void);


//Code implementation//
/*
 * Timer32_Init(void)
 *
 * Initalizes the timer to run on free running with interrupts
 * using MCLK for timing
 *
 * 3MHz-> 333.33ns/clk cycles
 * want to setup to for 38KHz
 * 38KHz -> 38000 cycles per seconds
 * 50% duty cycle ->
 * 19000 cycles per seconds ON
 * 19000 cycles per seconds OFF
 *  333.33 ns * 19000 = 0.00633327
 *  1/0.00633 = 157.89 *19000 = 3000030.003
 *  1/19000 -> 1500 clks
 *  1/19000 -> 1500 clks
 *
 *  ->setting 32 bit periodic mode
 *
 *
 */
void Timer32_Init(void){

    TIMER32_1->CONTROL = 0x00; //rest timer

    //setting timer to free-running mode
    TIMER32_1->CONTROL &= ~TIMER32_CONTROL_MODE;

    //select 32 bit counter operation
    TIMER32_1->CONTROL |= TIMER32_CONTROL_SIZE;

    //setting wrapping logic
    //TIMER32_1->CONTROL |= null;

    //setting interrupt enabled
    TIMER32_1->CONTROL |= TIMER32_CONTROL_IE;

    //setting prescale to 0
    TIMER32_1->CONTROL |= TIMER32_CONTROL_PRESCALE_0;

    //enable timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    //setting the loading register
    TIMER32_1->LOAD = 1500;

    //setting the BIGLOAD register
    TIMER32_1->BGLOAD = 1500; //50% duty cycle

    //clear any flag
    TIMER32_1->INTCLR = 0x00; // clear flag

    P2->DIR |= BIT0; //P2.0
    P2->OUT &= ~BIT0; //initialize to off

    lol = 0;
    return;
}


/*
 * Timer 32 enabling using the control register
 */
void Timer32_Start(void){

    //enable timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    return;
}


/*
 * Timer 32 disabling the timer using the control register
 */
void Timer32_Stop(void){

    //disable timer
    TIMER32_1->CONTROL &= ~TIMER32_CONTROL_ENABLE;

    return;
}


/*
 * Get the count value from the current value register
 */
int Timer32_getCount(void){

    //return current value register
    return TIMER32_1->VALUE;
}


/*
 * IQR Handler
 */
void T32_INT1_IQRHandler(void){
    TIMER32_1->INTCLR = 0x0;
    if(lol == 1){
        P2->OUT &= ~BIT0;
        TIMER32_1->BGLOAD = 1500;
        lol = 0;
    }
    else{
        P2->OUT |= BIT0;
        TIMER32_1->BGLOAD = 1500;
        lol = 1;
    }

}
