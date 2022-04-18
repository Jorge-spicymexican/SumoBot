/*
 * Simple_SM.h
 *
 *  Created on: Feb 14, 2022
 *      Author: jurado-garciaj
 */

#ifndef APPS_SIMPLE_SM_H_
#define APPS_SIMPLE_SM_H_


/* System Includes */
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "Motor.h"


/* Different states for sumo Bot */
typedef enum
{
    Start_State,
    Idle_State,
    Search_State,
    Attack_State,
    Line_Sensor_State,
    last_State

} eSystemState;


/* Different type of Events */
typedef enum
{
    ButtonNO_Event,
    ButtonYES_Event,
    Object_Detect_Event,
    Object_NotDetect_Event,
    Bounds_Detected_Event,
    Bounds_NotDetected_Event,
    Search_Timer_Event,
    Search_Done_Event,
    Last_Event


} eSystemEvent;

Motor *State( Motor *mt, eSystemState State, eSystemEvent Event);

#endif /* APPS_SIMPLE_SM_H_ */
