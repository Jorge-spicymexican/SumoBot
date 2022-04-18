/*
 * system.c
 *
 *  Created on: Dec 18, 2021
 *      Author: jurado-garciaj
 *      Objective: This file creates definitions and type def structs for
 *      pins being used on the MSP board. Should help us de-code beter and keep
 *      the main code cleaner.
 *
 *      See header file for module description.
 *
 *      Rev:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "system.h"

//public function prototypes
void SystemReset();
void SystemOn();
int Rounder(float num);
void blink(int Color);

//set all pins to input pins and outputs to 0 then blink Green LED
void SystemReset(){
    //setting Port 1-9 DIR @ input mode which is one
    // of all the pins that are connected to the Ports.
    // 1 means output and 0 means input.
    //P2 = 0X00
    P1->DIR &= ~(ALL);
    P2->DIR &= ~(ALL);
    P3->DIR &= ~(ALL);
    P4->DIR &= ~(ALL);
    P5->DIR &= ~(ALL);
    P6->DIR &= ~(ALL);
    P7->DIR &= ~(ALL);
    P8->DIR &= ~(ALL);
    P9->DIR &= ~(ALL);
    P10->DIR &= ~(ALL);

    //Set output pin to 0
    P1->OUT &= ~(ALL);
    P2->OUT &= ~(ALL);
    P3->OUT &= ~(ALL);
    P4->OUT &= ~(ALL);
    P5->OUT &= ~(ALL);
    P6->OUT &= ~(ALL);
    P7->OUT &= ~(ALL);
    P8->OUT &= ~(ALL);
    P9->OUT &= ~(ALL);
    P10->OUT &= ~(ALL);

    //turn on green led
    blink(GREEN);

    return;
}

//set all pins to outputs and on high and blink red led
void SystemOn(){

    //setting the DIR of All ports to 1
    P1->DIR |= (ALL);
    P2->DIR |= (ALL);
    P3->DIR |= (ALL);
    P4->DIR |= (ALL);
    P5->DIR |= (ALL);
    P6->DIR |= (ALL);
    P7->DIR |= (ALL);
    P8->DIR |= (ALL);
    P9->DIR |= (ALL);
    P10->DIR |= (ALL);

    //Set output pin to 1
    P1->OUT |= (ALL);
    P2->OUT |= (ALL);
    P3->OUT |= (ALL);
    P4->OUT |= (ALL);
    P5->OUT |= (ALL);
    P6->OUT |= (ALL);
    P7->OUT |= (ALL);
    P8->OUT |= (ALL);
    P9->OUT |= (ALL);
    P10->OUT |= (ALL);

    //turn on Red led
    blink(RED);

    return;
}


void blink(int Color){
    P2->DIR |= Color;
    // Here, we turn on the LED by putting a 1 in the pin's OUT register.
    P2->OUT = Color;  // turn the LED on

    return;
}

int rounding(float num){
    //Here we will round a number based on its decimal value
    if( num < 0){
        return num-0.6;
    }
    return num+0.6;
}

void button_init(void){
    //this does not setup the ADC conversion this will have to be
    //done in main_code using the other functions

    P1->DIR &= ~(0x10 | 0x02);
    P1->REN |= (0x10 | 0x02);
    P1->OUT |= (0x10 | 0x02);

    return;
}
