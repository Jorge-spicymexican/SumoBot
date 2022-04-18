/* platform includes */
#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

/*include header files */
#include "interrupt.h"
#include <Apps/LCD_System.h>
#include "msoe_lib_clk.h"
#include "msoe_lib_lcd.h"
#include "system.h"
#include "Motor.h"
#include "object_sensor.h"
#include "Apps/Line_sensors.h"
//#include "State_Machine.h"
#include "Simple_SM.h"


/*
 * main.c
 */
void main(void){

    //set frquency
    Clock_Init_48MHz();

    //stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //reset in pins to DIR 0 and output 0
    SystemReset();

    //Initalizing LCD
    LCD_Start();

    //Initalizing the motors
    Motor_Init();

    //Initalizing the Line Sensor
    Line_Sensor_Init();

    //clear lcd
    LCD_clear();


#ifdef COMP
    //preset the value for calibration values my accessing the value automatically
    Set_Cal(4.2, 4.2);

#else

    //calibration function for line sensor
    Line_Sensor_Cal();
#endif

    //clear lcd
    LCD_clear();

    int delay = TRUE;


    while( delay == TRUE){
        button_init();
        int count;
        //do nothing but check for button
        while(  ( ((P1->IN & RB) ==  0 ) ||  ( (P1->IN & LB) ==  0 ) ) == 1){
            __delay_cycles(500000); //1 seconds
            LCD_UNSIGNED_DEC_PRINT(COL7,ROW6,count);
            count++;

        }
        if( count >= 2){
            __delay_cycles(125000000); //5 seconds
            delay = FALSE;

        }
        else{
            delay = TRUE;
        }
    }

    //clear lcd
    LCD_clear();

    //start lcd screen
    LCD_Start();

    //Pre configured LCD screen display
    LCD_PRESET();

    //init object sensor
    Object_sensor_init();

    //setting interrupt registers and priority
    NVIC_setup();

    //Globle enabling for interrupts
    __enable_interrupts();

    //start ADC
    ADC14->CTL0 |= ADC14_CTL0_SC;

    //creating motor type def
    Motor sumo_bot;

    CURRENT_STATE curr_state;
    LAST_STATE last_state;



#ifdef COMP


    Direction Past_object_response = NO_OBJECT;

    Direction line_response = NO_SENSOR;

    Direction object_response = NO_OBJECT;

    //Pre-configured LCD screen display
    LCD_PRESET();

    while(1){

        blink(GREEN);

        object_response = NO_OBJECT;

        Line_Sensor_Start();

        line_response = Line_logic();

        object_response = Object_Response();


        if( line_response == NO_SENSOR ){

            //run normal code

            if( object_response != NO_OBJECT){


                Past_object_response = object_response;


                Motor_Input(&sumo_bot,STOP,30,100);
                Motor_Input(&sumo_bot,object_response,65,90);
                curr_state = Motor_Current(&sumo_bot);
                last_state = Motor_Previous(&sumo_bot);
                _0_25_sec;

                // check if the object is still around

                object_response = Object_Response();

                if( object_response != NO_OBJECT){


                    Past_object_response = object_response;


                    Motor_Input(&sumo_bot,STOP,30,100);
                    Motor_Input(&sumo_bot,object_response,65,90);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                    _0_25_sec;


                }
            }
            else{

                static int count;

                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("OBJECT: NO ");

                //30 degrees of revolution
                if( count < turn_value ){

                    Motor_Input(&sumo_bot,STOP,30,90);
                    Motor_Input(&sumo_bot,START,60,90);
                    Motor_Input(&sumo_bot,ROTATE_RIGHT,65,90);
                    turn_rev;

                }
                else if( (count < forward_value) && ( count >= turn_value ) ){

                    Motor_Input(&sumo_bot,FORWARD,30,90);
                    Motor_Input(&sumo_bot,START,65,90);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                    __delay_cycles(150000);

                }
                else if( (count < turn_value+forward_value) && (count >= forward_value) ){

                    Motor_Input(&sumo_bot,STOP,30,90);
                    Motor_Input(&sumo_bot,START,65,90);
                    Motor_Input(&sumo_bot,ROTATE_LEFT,65,90);
                    turn_rev;

                }

                else{

                    count = 0;

                }

                count++;
            }
        }
        else{
            //turn on a red led as an indicator
            blink(RED);
            //run line sensor information
            Motor_Input(&sumo_bot,line_response,60,95);
            left_black = FALSE;
            right_black = FALSE;

        }

    }

#endif


#ifdef State_Machine
    eSystemState eNextState = Start_State;
    eSystemEvent eNewEvent;

    while(1){


        char count;
        //do nothing but check for button
        while(  ( ((P1->IN & RB) ==  0 ) ||  ( (P1->IN & LB) ==  0 ) ) == 1){
            __delay_cycles(100000); //1 seconds
            count++;

        }
        if( count >= 3){
            eNewEvent == ButtonYES_Event;
        }
        else{
            eNewEvent == ButtonNO_Event;
        }


        //go ahead and read the next even
        //eSystemEvent eNewEvent = read_event();

        if((eNextState < last_State) && (eNewEvent < Last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEventHandler != NULL))
        {
            // function call as per the state and event and return the next state of the finite state machine
            eNextState = (*asStateMachine[eNextState].pfStateMachineEventHandler)();
        }
        else
        {
            //Invalid
            printf("Invalid!\n");
        }

    }

#endif


#ifdef EASY_COMPETITION


    while(1){
        Direction line_response = NO_SENSOR;

        Direction object_response = NO_OBJECT;

        //Pre configured LCD screen display
        LCD_PRESET();

        while(1){

            Line_Sensor_Start();
            line_response = Line_logic();
            object_response = Object_Response();

            if( line_response == NO_SENSOR ) {

                //run normal code

                if( object_response != NO_OBJECT){

                    Motor_Input(&sumo_bot,STOP,30,100);
                    Motor_Input(&sumo_bot,object_response,50,100);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                    _0_5_sec;

                }
                else{

                    static int count;

                    LCD_goto_xy(COL1,ROW6);
                    LCD_print_str("OBJECT: NO ");

                    //30 degrees of revolution
                    if( count < turn_value ){

                        Motor_Input(&sumo_bot,STOP,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        Motor_Input(&sumo_bot,ROTATE_RIGHT,60,100);
                        turn_rev;

                    }
                    else if( (count < forward_value) && ( count >= turn_value ) ){

                        Motor_Input(&sumo_bot,FORWARD,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        curr_state = Motor_Current(&sumo_bot);
                        last_state = Motor_Previous(&sumo_bot);
                        turn_rev;

                    }
                    else if( (count < turn_value+forward_value) && (count >= forward_value) ){

                        Motor_Input(&sumo_bot,STOP,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        Motor_Input(&sumo_bot,ROTATE_LEFT,60,100);
                        turn_rev;

                    }
                    else{

                        count = 0;

                    }

                    count++;
                }
            }
            else{
                //turn on a red led as an indicator
                blink(RED);
                //run line sensor information
                Motor_Input(&sumo_bot,line_response,60,100);
                blink(GREEN);

            }

        }

#endif


#ifdef _5Delay_Behavior_Test

        Direction line_response = NO_SENSOR;

        Direction object_response = NO_OBJECT;

        //Pre configured LCD screen display
        LCD_PRESET();

        while(1){

            printf("Going to Idle statte.\n");
            Line_Sensor_Start();
            line_response = Line_logic();
            object_response = Object_Response();
            _5_sec;

            printf("Object response is: %d.\n", object_response);

        }

#endif


#ifdef Search_Behavior_Test

        Direction line_response = NO_SENSOR;

        Direction object_response = NO_OBJECT;

        //Pre configured LCD screen display
        LCD_PRESET();

        while(1){

            Line_Sensor_Start();
            line_response = Line_logic();
            object_response = Object_Response();

            if( line_response == NO_SENSOR ) {

                //run normal code

                if( object_response != NO_OBJECT){

                    Motor_Input(&sumo_bot,STOP,30,100);
                    Motor_Input(&sumo_bot,object_response,60,100);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                    _0_25_sec;

                }
                else{

                    static count;

                    LCD_goto_xy(COL1,ROW6);
                    LCD_print_str("OBJECT: NO ");

                    //30 degrees of revolution
                    if( count < turn_value ){

                        Motor_Input(&sumo_bot,STOP,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        Motor_Input(&sumo_bot,ROTATE_RIGHT,30,100);
                        turn_rev;

                    }
                    else if( (count < forward_value) && ( count >= turn_value ) ){

                        Motor_Input(&sumo_bot,FORWARD,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        curr_state = Motor_Current(&sumo_bot);
                        last_state = Motor_Previous(&sumo_bot);
                        turn_rev;

                    }
                    else if( (count < turn_value+forward_value) && (count >= forward_value) ){

                        Motor_Input(&sumo_bot,STOP,30,100);
                        Motor_Input(&sumo_bot,START,60,100);
                        Motor_Input(&sumo_bot,ROTATE_LEFT,30,100);
                        turn_rev;

                    }
                    else{

                        count = 0;

                    }

                    count++;
                }
            }
            else{
                //turn on a red led as an indicator
                blink(RED);
                //run line sensor information
                Motor_Input(&sumo_bot,line_response,30,100);
                left_black = FALSE;
                right_black = FALSE;
                blink(GREEN);

            }

        }
#endif


#ifdef Object_Behavior_Test

        Direction line_response = NO_SENSOR;

        Direction object_response = NO_OBJECT;

        while(1){

            Line_Sensor_Start();

            line_response = Line_logic();

            object_response = Object_Response();

            if( line_response == NO_SENSOR ) {

                if( object_response != NO_OBJECT){
                    Motor_Input(&sumo_bot,STOP,30,100);
                    Motor_Input(&sumo_bot,object_response,60,100);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                    _0_25_sec;

                }
                else{
                    printf("object not detected.\n");
                    //run normal code
                    Motor_Input(&sumo_bot,START,30,100);
                    Motor_Input(&sumo_bot,FORWARD,60,100);
                    curr_state = Motor_Current(&sumo_bot);
                    last_state = Motor_Previous(&sumo_bot);
                }
            }
            else{
                //run line sensor information
                Motor_Input(&sumo_bot,line_response,30,100);
                left_black = FALSE;
                right_black = FALSE;
            }

        }

#endif


#ifdef LineSensor_Behavior_Test

        Direction line_response = NO_SENSOR;

        while(1){

            Line_Sensor_Start();

            line_response = Line_logic();


            if( line_response == NO_SENSOR ) {
                //run normal code
                Motor_Input(&sumo_bot,START,30,100);
                Motor_Input(&sumo_bot,FORWARD,60,100);
                curr_state = Motor_Current(&sumo_bot);
                last_state = Motor_Previous(&sumo_bot);
            }
            else{
                //run line sensor information
                Motor_Input(&sumo_bot,line_response,30,100);
                left_black = FALSE;
                right_black = FALSE;
            }

        }

#endif


#ifdef Motor_Behavior_Test
        while(1){

            for(Direction i = FORWARD; i < STOP; i++){
                Motor_Input(&sumo_bot,STOP,50,100);
                Motor_Input(&sumo_bot,i,50,100);
                _2_sec;
                Motor_Input(&sumo_bot,STOP,50,100);
                _5_sec;
            }
        }

#endif


#ifdef     Lab_EXAM2

        ////once the user calibrated the line sensors follow these protocol

        Direction line_response = NO_SENSOR;

        Direction object_response = NO_OBJECT;


        //delay for 5 seconds
        _5_sec;

        Direction choice;



        choice = FORWARD;


        //go straight
        while(1){


            Line_Sensor_Start();

            line_response = Line_logic();
            object_response = Object_Response();

            line_response = Line_logic();

            if( line_response == NO_SENSOR ) {

                blink(BLUE);
                Motor_Input(&sumo_bot,START,30,100);
                Motor_Input(&sumo_bot, choice, 60, 60);
                curr_state = Motor_Current(&sumo_bot);
                last_state = Motor_Previous(&sumo_bot);

            }
            else{

                blink(GREEN);

                if( choice == FORWARD){

                    Motor_Input(&sumo_bot,FORWARD,60,100);
                    _0_5_sec;

                    while( object_response == NO_OBJECT){
                        //this is shitty but need the grade LOL
                        //if line sensor is detect stay in this else statement until an object is detected
                        //turn on a red led as an indicator
                        blink(RED);
                        //run line sensor information
                        Motor_Input(&sumo_bot,LEFT_SENSOR,30,100);
                        curr_state = Motor_Current(&sumo_bot);
                        last_state = Motor_Previous(&sumo_bot);

                        object_response = Object_Response();
                    }

                    Motor_Input(&sumo_bot,STOP,60,100);
                    Motor_Input(&sumo_bot, BACKWARD, 60, 100);
                    _0_5_sec;
                    _0_5_sec;
                    line_response = NO_SENSOR;
                    choice = BACKWARD;

                }
                else if ( choice == BACKWARD){

                    blink(GREEN);
                    Motor_Input(&sumo_bot,STOP,60,100);
                    Motor_Input(&sumo_bot, LEFT_SENSOR, 60, 100);
                    _0_5_sec;
                    _0_25_sec;
                    Motor_Input(&sumo_bot, FORWARD, 60, 100);
                    _0_5_sec;
                    choice = FORWARD;

                    while( object_response == NO_OBJECT){
                        //this is shitty but need the grade LOL
                        //if line sensor is detect stay in this else statement until an object is detected
                        //turn on a red led as an indicator
                        blink(RED);
                        Motor_Input(&sumo_bot, FORWARD, 60, 30);
                        turn_rev;
                        object_response = Object_Response();
                    }
                    //object detect turn while led and stop

                    choice = STOP;

                    //clear lcd
                    LCD_clear();

                }
                else{
                    //do tnothing;

                    blink( RED|GREEN|BLUE);
                    Motor_Input(&sumo_bot,STOP,00,100);

                    //clear lcd
                    LCD_clear();
                    LCD_goto_xy(COL1,ROW3);
                    LCD_print_str("DONE");

                }
            }

        }//end while


#endif


#ifdef Lab_EXAM1


        LCD_Lab_EXAM();


        // enumeration for state


        typedef enum{
            START,
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            FINISH
        } state;


        state heyp = START;
        //testing for rotation
        heyp = START;
        int Check_off = TRUE;

        while(Check_off == TRUE){

            //go to R6 & C9 and printf STEP
            LCD_goto_xy(COL1,ROW6);


            //step 1 stay in idle state
            //setting the register threshold values to a specific value
            switch(heyp){
            case START:
                LCD_goto_xy(COL1,ROW6);
                Motor_Input(&sumo_bot,STOP,30,100);
                _5_sec;
                break;
            case ONE:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("ONE  ");
                Motor_Input(&sumo_bot,SPIN_LEFT,30,100);
                _2_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case TWO:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("TWO  ");
                Motor_Input(&sumo_bot,FORWARD,30,50);
                _1_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case THREE:   //spining 45 degrees
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("THREE");
                Motor_Input(&sumo_bot,ROTATE_LEFT,30,100);
                _0_5_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case FOUR:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("FOUR ");
                Motor_Input(&sumo_bot,BACKWARD,30,100);
                _4_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case FIVE:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("FIVE ");
                Motor_Input(&sumo_bot,ROTATE_RIGHT,30,100);
                _0_5_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case SIX:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("SIX  ");
                Motor_Input(&sumo_bot,FORWARD,30,100);
                _1_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case SEVEN:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("SEVEN");
                Motor_Input(&sumo_bot,SPIN_RIGHT,30,100);
                _2_sec;
                Motor_Input(&sumo_bot,STOP,30,100);
                break;
            case FINISH:
                _0_5_sec;
                LCD_goto_xy(COL1,ROW6);
                LCD_print_str("     ");
                Motor_Input(&sumo_bot,STOP,30,100);
                _1_sec;
                Check_off = FALSE;
                break;
            }

            heyp = heyp + 1;

#endif


        }//end main
