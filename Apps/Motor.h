/*
 * Motor.h
 *
 *  Created on: Dec 19, 2021
 *      Author: jurado-garciaj
 *      Objective: This file creates definitions and type def structs for
 *      motor on the sumo bot.
 *
 *  inputs:
 *      Timer_ANl
 *      Motor Speed
 *      Motor Direction
 *
 *
 *  outputs:
 *      Function output and state machine
 *      Rev:
 *          0: Finishing setup information for private functions 1/2/2022 JJ
 *          1: Extra function like Resetting the motor functions to Forward direction w/ Motor stop 1/2/2022 JJ
 *
 */

#ifndef APPS_MOTOR_H_
#define APPS_MOTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"

//enumerations
typedef enum{
    FORWARD,   //0
    BACKWARD, //1
    LEFT,   //2
    RIGHT, //3
    SPIN_LEFT, //4
    SPIN_RIGHT, //5
    ROTATE_LEFT, //6
    ROTATE_RIGHT, //7
    STOP,  //8
    START,  //9
    LEFT_SENSOR, //10
    RIGHT_SENSOR, //11
    BOTH_SENSOR, //12
    NO_SENSOR, //13
    NO_OBJECT, //14
    UH_OH      //15
} Direction;

typedef struct{
    Direction C;
} CURRENT_STATE;

typedef struct{
    Direction L;
} LAST_STATE;

//definitions
#define STBY BIT3
#define AIN1 BIT0
#define AIN2 BIT6
#define BIN1 BIT5
#define BIN2 BIT7
#define PWMA BIT4
#define PWMB BIT5
#define F_sys 904
#define RIGHT_SIDE 1
#define LEFT_SIDE 0



//type def struct for motor

typedef struct{
    CURRENT_STATE cs;
    LAST_STATE ls;
    Direction direction;
    int freq;
    int duty_cycle;
} Motor;


//functions prototypes
void Motor_Init();
Motor* Motor_Input(Motor *mt, Direction direction, int speed, int torque );
CURRENT_STATE Motor_Current(Motor *mt);
LAST_STATE Motor_Previous(Motor *mt);

/*
 * Initalizes all the GPIO pins for the motor and Sets the freuqency for all of the motors to 100kHz
 * Futhermore, this will also give set the Motor in the STOP mode by setting the bits high
 *
 * Motor Init() Initaliazes all of the GPIO Pins for the Motor and initalies the PWM Waveform
 */


/*
 * User inputed function user will have to decided a direction, speed, and torwue for the motor.
 * This will then control the frequency duty cycle of the controler and also the GPIO pins for the Motor Driver
 * Uses a simple case statement as the state machine for the code and only interrupts used are the PWMA and PWMB.
 *
 * Motor* Motor_Input(Motor *mt, Direction direction, int speed, int torque );
 */


/*
 * Gives the User their current state of the Motor drivers inside the Motor_input( agrs) function
 *
 * CURRENT_STATE Motor_Current(Motor *mt);
 */


/*
 * Gives the User there previous state of the motors inside the Motor_input (agrs) function
 *
 * LAST_STATE Motor_Previous(Motor *mt);
 */

#endif /* APPS_MOTOR_H_ */
