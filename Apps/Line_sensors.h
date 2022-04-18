/*
 * Line_sensors.h
 *
 *  Created on: Jan 2, 2022
 *      Author: jurado-garciaj
 *      Objective: This file creates definitions and type def structs for
 *      line sensors in the sumobot project.
 *
 *      Rev:
 *          1 1/15/2022  rid of global variables and adding comments
 *
 *
 *
 */

#ifndef APPS_LINE_SENSORS_H_
#define APPS_LINE_SENSORS_H_

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "motor.h"

/*   global variables   */
//this creates values into a float value after conversion
float left_val_black;
float right_val_black;

//calibrated values from cal settings
float cal_left;
float cal_right;

//true of false values for line sensors
int left_black;
int right_black;

//defines
#define LEFT_PWR_PIN BIT5 //P8.5
#define RIGHT_PWR_PIN BIT6 //P8.6
#define LEFT_READ_PIN BIT5 //P5.5 A0
#define RIGHT_READ_PIN BIT0 //P5.0 A5


#define RB  0x02
#define LB 0x10

//functions prototypes
void Line_Sensor_Init(void);
void Line_Sensor_Cal(void);
void Line_Sensor_Stop(void);
void Line_Sensor_Start(void);
Direction Line_logic(void);
void Set_Cal(float left, float right);


/*
 * Starts GPIO pins for Line Sensor and sets the Registers needed for ADC conversion
 *
 * void Line_Sensor_Init(void);
 */


/*
 * Calibrates the Line Sensors on the sumo bot competitions this will be uses when using
 * a different platform or even different colors
 *
 * void Line_Sensor_Cal(void);
 */


/*
 * Stops the Line Sensors from firing could be used in program
 * just implemented just in case
 *
 * void Line_Sensor_Stop(void);
 */


/*
 * Starts the Line sensors to read values from the Reflective lenses
 *
 * void Line_Sensor_Start(void);
 */


/*
 * Line sensors logic for settings how the pins are going to be operating this will return
 * direction value to be used inside my motor input
 *
 * int Line_logic(void);

 */


/*
 * Line sensors calibrated values will be automatically set instead of automatic values
 * This is due to competition code and ease of access
 *
 * void Set_Cal(void);
 */

#endif /* APPS_LINE_SENSORS_H_ */
