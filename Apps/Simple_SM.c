/* System Includes */
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"

/* Local Includes */
#include "State_Machine.h"
#include "object_sensor.h"
#include "Motor.h"
#include "Line_sensors.h"


Motor *State( Motor *mt, eSystemState State, eSystemEvent Event){
    switch(Event){
       case FORWARD:
}
