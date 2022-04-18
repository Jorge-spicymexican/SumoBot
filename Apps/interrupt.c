/*
 * interrupt.c
 *
 *  Created on: Jan 9, 2022
 *      Author: jurado-garciaj
 */


#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "Apps/Line_sensors.h"
#include "Apps/Motor.h"
#include "Apps/interrupt.h"
#include "Apps/Timer32.h"

//function protypes
void NVIC_setup();


/*
 * this funciton is the NVIC priority and setup
 */
void NVIC_setup(){

    //logic for timer A0 interrupts
    //enable timer A0
    //NVIC->IP[9] |= 0x40; // Set a priority
    //NVIC->ISER[0] |= (1<<9); //ISER0 set to 9

    //logic for ADC register and logic
    //enable ADC
    //Note: ADC is INTISR(24)
    //NVIC->IP[24] |= 0x30; //what is this priority bullshit
    NVIC->ISER[0] |= (1<<24); //ISER0 set to 24

    //logic for timer A2
    //NVIC->ISER[0] |= (1<<13); //ISER0 set to 13

    //logic for timer A1
    NVIC->ISER[0] |= (1<<11); //ISER0 set to 11

    return;

}

