/*
 * State Machine.h
 *
 *  Created on: Jan 14, 2022
 *      Author: jurado-garciaj
 *
 *      This is just to fuck around and have something that
 *      looks neat for competition.
 *
 *      Rev:
 *           2/15/2022  rid of global variables and adding comments
 *
 */

#ifndef APPS_STATE_MACHINE_H_
#define APPS_STATE_MACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"


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


//typedef of function pointer
typedef eSystemState (*pfEventHandler)(void);

//creating a structure of state and event with event handler
typedef struct
{
    eSystemState eStateMachine;
    eSystemEvent eStateMachineEvent;
    pfEventHandler pfStateMachineEventHandler;
} sStateMachine;


void read_event( void );

#endif /* APPS_STATE_MACHINE_H_ */
