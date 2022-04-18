/*
 * object_sensor.c
 *
 *  Created on: Dec 19, 2021
 *      Author: jurado-garciaj
 *      Objective:
 *
 *  See Header file for more information
 *
 *      Rev:
 *
 */


#include "system.h"
#include "object_sensor.h"
#include "motor.h"
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"


//public function prototypes
void Object_sensor_init();
Direction Object_Response();


//private function prototypes
void Receiver_init();
void Emitter_init();
void TAI_SETUP();
void TA2_SETUP();


/*
 *  object sensor init functions for setting up pins and etc
 */
void Object_sensor_init(){

    //init pwm pins
    Receiver_init();

    //init GPIO pins for new symbol
    Emitter_init();

    //display red as pwm init

    //timer setup
    TAI_SETUP();

    //timer setup for TA2.0
    TA2_SETUP();

    //display nothing led as an indicator that setup is done
    blink(BLUE);

    return;
}


/*
 * will set a direction bit back to main in order to get processed
 * void Object_Response();
 */
Direction Object_Response(){

    Direction return_val;

#ifdef COMP

    if((receive & 0x03) == 0x00){

        //printf("all object sensors are active.\n");

        blink( (BLUE|RED|GREEN) ); //white light

        //printf("object all around us.\n");
        return_val = FORWARD;


    }
    else if( ( ( receive & BIT0) == 0x00)){

        /* status 01000010  left receiver low means its being read
         *      & 00000001
         * result 00000000
         *
         * check  00000001
         *
         * if high then the result will be 0x00 else it will be the 0xFF
         */

        //printf("left object sensor is active.\n");

        blink( (BLUE|RED) );   //purple light

        //printf("object left.\n");

        return_val = LEFT;


    }
    else if(  ( receive & BIT1) == 0x00 ){

        /* status 01000001  right reciever low means its being read
         *      & 00000010
         * result 00000000  -output is high
         *
         *check   00000010
         * if high then the result will be 0x02 else it will be the 0x00
         */

        // printf("right object sensors is active.\n");
        // printf("object right.\n");

        blink( (GREEN|RED) );  //no object turn yellow

        return_val = RIGHT;

    }
    else{
        // printf("NO object sensors are active.\n");

        blink(GREEN);  //no object turn green
        return_val = NO_OBJECT;

    }

    return return_val;

#endif

#ifdef three_oj
    //read the reciever pin for checking its value and status
    if((receive & 0x13) == 0x00){

        //printf("all object sensors are active.\n");

        blink( (BLUE|RED|GREEN) ); //white light

        //printf("object all around us.\n");
        return_val = FORWARD;


    }
    else if( ( receive & 0x03) == 0x00){

        //both receiver are sensing do
        blink( (BLUE| GREEN )); //blik indigo

        //printf("object font of us.\n");
        return_val = FORWARD;

    }
    else if( ( ( receive & 0x11) == 0x00) || ( (receive & LEFT_RECIEVER) == 0x00) ){

        /* status 01000010  left receiver low means its being read
         *      & 00000001
         * result 00000000
         *
         * check  00000001
         *
         * if high then the result will be 0x00 else it will be the 0xFF
         */

        //printf("left object sensor is active.\n");

        blink( (BLUE|RED) );   //purple light

        //printf("object left.\n");

        return_val = LEFT;


    }
    else if(  ( ( receive & 0x12) == 0x00) || ( (receive & RIGHT_RECIEVER) == 0x00) ){

        /* status 01000001  right reciever low means its being read
         *      & 00000010
         * result 00000000  -output is high
         *
         *check   00000010
         * if high then the result will be 0x02 else it will be the 0x00
         */

        // printf("right object sensors is active.\n");
        // printf("object right.\n");

        blink( (GREEN|RED) );  //no object turn yellow

        return_val = RIGHT;

    }
    else if( (receive & BACK_IR_RECEIVER) == 0x00 ){
        //sensing an object behind us

        //print object behind us
        //printf("object behaind us.\n");
        blink( BLUE );

        return_val = UH_OH;


    }
    else{
        // printf("NO object sensors are active.\n");

        blink(GREEN);  //no object turn green
        return_val = NO_OBJECT;

    }

    return return_val;
#endif


}


/*
 * PWM_init() for recevier values
 */
void Receiver_init(){

    //setting ports to be an input
    P6->DIR &= ~LEFT_RECIEVER;
    P6->DIR &= ~RIGHT_RECIEVER;
    P6->DIR &= ~BACK_IR_RECEIVER;

    //set output to zero
    P6->OUT &= ~LEFT_RECIEVER;
    P6->OUT &= ~RIGHT_RECIEVER;
    P6->OUT &= ~BACK_IR_RECEIVER;

    //ENSURE pullx not enabled
    P6->REN &= ~LEFT_RECIEVER;
    P6->REN &= ~RIGHT_RECIEVER;
    P6->REN &= ~BACK_IR_RECEIVER;


    //power for receiver values

    //setting ports DIR to output
    P7->DIR |= LEFT_R_POWER;
    P7->DIR |= RIGHT_R_POWER;
    P7->DIR |= Back_R_POWER;
    //set output to high
    P7->OUT |= LEFT_R_POWER;
    P7->OUT |= RIGHT_R_POWER;
    P7->OUT |= Back_R_POWER;

    return;

}


/*
 * This functions sets up the pins for TA2 pwm conversion values
 */
void Emitter_init(){

    //set DIR High
    P5->DIR |= ( LEFT_IR_EMIT|RIGHT_IR_EMIT );
    //Set SEL0 High And SEL1 LOW
    P5->SEL0 |= ( LEFT_IR_EMIT|RIGHT_IR_EMIT );
    P5->SEL1 &= ~( LEFT_IR_EMIT|RIGHT_IR_EMIT );
    //Set OUT High
    P5->OUT |= ( LEFT_IR_EMIT|RIGHT_IR_EMIT );

    P6->DIR |= BACK_IR_EMIT;
    P6->SEL0 |= BACK_IR_EMIT;
    P6->SEL1 &= ~BACK_IR_EMIT;
    P6->OUT |= BACK_IR_EMIT;

    P6->DIR |= RIGHT_IR_EMIT;
    P6->SEL0 |= RIGHT_IR_EMIT;
    P6->SEL1 &= ~RIGHT_IR_EMIT;
    P6->OUT |= RIGHT_IR_EMIT;

    return;
}



/*
 * sets the timers up in continous mode for this object sensor
 */
void TAI_SETUP(){

    //setting up control register timer
    //up mode with SMCLK clock and clear flag in the flag
    TIMER_A1->CTL |= TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    //enable interrupt for this register
    TIMER_A1->CCTL[1] |= (TIMER_A_CCTLN_CCIE);

    //enable interrupt for this register
    TIMER_A1->CCTL[2] |= (TIMER_A_CCTLN_CCIE);

    TIMER_A1->CCR[0] = 65535;

    TIMER_A1->CCR[1] = 0;

    TIMER_A1->CCR[2] = 4896;


    return;
}


/*
 * Timer A2_Setup for control logic and relay.
 */
void TA2_SETUP(){

    /*
     * setting up pins for TA2
     */

    //  SMLK || Up_mode || input divider by 1
    TIMER_A2->CTL |= (TIMER_A_CTL_SSEL__SMCLK) | TIMER_A_CTL_ID_0 | TIMER_A_CTL_MC__UP;


    TIMER_A2->CCTL[1] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set

    TIMER_A2->CCTL[2] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set

    TIMER_A2->CCTL[3] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set

    TIMER_A2->CCTL[4] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set


    TIMER_A2->EX0 |= 0x02; //divide by 3

    TIMER_A2->CCR[0] = (4*980)/38;

    //set all motor to zero in order to have them off
    TIMER_A2->CCR[1] = 0;

    TIMER_A2->CCR[2] = 0;

    TIMER_A2->CCR[3] = 0;

    TIMER_A2->CCR[4] = 0;

    int current_val =  TIMER_A2->CCR[0];

    current_val = (current_val*(50))/100;

    TIMER_A2->CCR[1] = current_val;

    TIMER_A2->CCR[2] = current_val;

    TIMER_A2->CCR[3] = current_val;

    TIMER_A2->CCR[4] = current_val;


    return;
}



//Timer ISR
void TA1_N_IRQHandler(void){


    int iv = TIMER_A1->IV;

    if (iv == 0x02){

        // P5->OUT |= ( LEFT_IR_EMIT|RIGHT_IR_EMIT );
        // P6->OUT |= ( BIT6 );

    }
    if (iv == 0x04){
        //if the interrupt was caused by CCR2 which means its CCR[2]=4896

        //P5->OUT &= ~( LEFT_IR_EMIT|RIGHT_IR_EMIT );
        //P6->OUT &= ~( BIT6 );

        receive = ( (P6->IN) & (LEFT_RECIEVER|RIGHT_RECIEVER|BACK_IR_RECEIVER) );


    }//end if

}

/*
 * this can be deleted since this does not work great for my implemenation
 */
/*
void TA2_N_IRQHandler(void){

    //if nothing is causing the interrupt toggle P4.2
    //the 16 cycles is caused from TA0 handler.
    //if no interrupt was caused from TA1 then code would be just
    //create 38kz waves and only the wave on and off.

    int iv = TIMER_A2->IV;
    P4->OUT ^=  RIGHT_IR_EMIT; //toggles the 38k signal
    P4->OUT ^=  LEFT_IR_EMIT; //toggles the 38k signal

}
 */
