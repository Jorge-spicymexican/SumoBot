/*
 * State_Machine Code for what is going on
 * and the function call for every handler
 */

/* System Includes */
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"

/* Local Includes */
#include "State_Machine.h"
#include "object_sensor.h"
#include "Motor.h"
#include "Line_sensors.h"



// function call to when the button is not pressed
eSystemState ButtonNoHandler(void){

    return Start_State;
}


// function to call when the button is pressed
eSystemState ButtonYESHandler(void){

    __delay_cycles(125000000); //5 seconds
    return Idle_State;

}


// function to do when idle state and how to move to other state machines
eSystemState Object_NotDetectHandler(void){

    return Idle_State;
}


//function call to object_detect and move to other state
eSystemState Object_DetectHandler(void){

    return Attack_State;
}


//function to bounds not detected
eSystemState Bounds_NotDetectedHandler(void){

    return Idle_State;
}


eSystemState Bounds_DetectedHandler(void){

    return Line_Sensor_State;
}


//function for timer to for object
eSystemState Search_TimerHandler(void){

    return Search_State;
}


//function for timer as expired
eSystemState Search_DoneHandler(void){

    return Idle_State;
}


//initalize the array of structions iwth statea and events with proper handleing
sStateMachine asStateMachine[] =
{

 { ButtonNO_Event, ButtonNoHandler},
 { ButtonYES_Event, ButtonYESHandler},
 { Object_Detect_Event, Object_DetectHandler},
 { Object_NotDetect_Event, Object_NotDetectHandler},
 { Bounds_Detected_Event, Bounds_DetectedHandler},
 { Bounds_NotDetected_Event, Bounds_NotDetectedHandler},
 { Search_Timer_Event, Search_TimerHandler},
 { Search_Done_Event, Search_DoneHandler}

};

void read_event( void ){


}

