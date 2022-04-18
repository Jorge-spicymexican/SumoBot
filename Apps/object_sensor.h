/*
 * object_sensor.h
 *
 *  Created on: Jan 12, 2022
 *      Author: jurado-garciaj
 */

#ifndef APPS_OBJECT_SENSOR_H_
#define APPS_OBJECT_SENSOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "system.h"
#include "motor.h"


//type def declarations
//enumerations
int receive; //recive global variable


#define LEFT_IR_EMIT BIT6
#define RIGHT_IR_EMIT BIT7
#define BACK_IR_EMIT BIT6

#define BACK_IR_RECEIVER BIT4
#define LEFT_RECIEVER BIT0
#define RIGHT_RECIEVER BIT1

#define LEFT_R_POWER BIT6
#define RIGHT_R_POWER BIT7
#define Back_R_POWER BIT4


//functions prototypes
void Object_sensor_init();
Direction Object_Response();

/*
 * Initalizes all the GPIO pins for the object sensor and Sets the freq to be at 38KHz
 *
 * Object_sensor_init(); Initaliazes all of the GPIO Pins for the Motor and initalies the PWM Waveform
 */


/*
 * This is the object response functions that will send a direction return value back to main
 * this will will get passed into a motor functions in order to get ran and controlled
 *
 * void Object_Response();
 */


#endif /* APPS_OBJECT_SENSOR_H_ */
