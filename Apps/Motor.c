/*
 * Motor.c
 *
 *  Created on: Dec 19, 2021
 *      Author: jurado-garciaj
 *      Objective: This file creates definitions and type def structs for
 *      motor on the sumo bot.
 *
 *  See Header file for more information
 *
 *  inputs:
 *      Timer_ANl
 *      Motor Speed
 *      Motor Direction
 *
 *
 *  outputs:
 *      Function output and state machine
 *
 *      Rev:
 *
 */


#include "Motor.h"
#include "math.h"
#include "system.h"
#include <Apps/LCD_System.h>
#include "msoe_lib_lcd.h"


//public function prototypes
void Motor_Init();
Motor* Motor_Input(  Motor *mt, Direction direction, int speed, int torque );
CURRENT_STATE Motor_Current(Motor *mt);
LAST_STATE Motor_Previous(Motor *mt);


//private function prototypses
void Motor_GO();
void Motor_Stop();
void Motor_Rev();
void Motor_FWD();
void Motor_Left();
void Motor_Right();
void Motor_Speed(int speed);
int Set_Duty(int duty, int loc);
void TA0_N_IRQHandler(void);
void PWMA_init();
void PWMB_init();
void TIMER_SETUP();
int Frequency();
int overflow(int num);




/*  motor intializes motor pins to DIR & OUT = 1
    this also sets the pins used for the motor controller such as STBY and etc
 */
void Motor_Init(){

    PWMA_init();
    PWMB_init();

    //setting STBY PIN to DIR 0 and OUT HIGH
    P2->DIR |= STBY;
    Motor_Stop();

    //Setting the AIN2,AIN1,BIN2,BIN1 AS OUTPUT
    P3->DIR |= (AIN2|AIN1|BIN2|BIN1);
    P3-> OUT &= ~(AIN2|AIN1|BIN2|BIN1);

    //Set up Timers
    TIMER_SETUP();

    //printing frequency
    int lol;
    lol = Frequency();
    lol -= 1;

    return;
}


//stops the motor from rotating by setting the Standby bit to 0 instead of 1
void Motor_Stop(){
    //Set STANBY PIN OUTPUT LOW
    P2->OUT &= ~STBY;

    return;
}


//allows the motor to rotate by setting the STBY BIT High
void Motor_GO(){
    //set STANBY PIN OUTPUT HIGH
    P2->OUT |= STBY;

    return;
}


//allows the user to make the motors go into reverse
void Motor_Rev(){
    P3->OUT |= AIN2;
    P3->OUT &= ~AIN1;
    P3->OUT |= BIN1;
    P3->OUT &= ~BIN2;
    Motor_GO();

    return;
}


//allows the user to make the motors go into forwards
void Motor_FWD(){
    P3->OUT &= ~AIN2;
    P3->OUT |= AIN1;
    P3->OUT &= ~BIN1;
    P3->OUT |= BIN2;
    Motor_GO();

    return;
}


//setting the pins for move the robot left or spin left
void Motor_Left(){

    P3->OUT &= ~AIN2;
    P3->OUT |= AIN1;
    P3->OUT |= BIN1;
    P3->OUT &= ~BIN2;
    Motor_GO();

    return;
}


//setting the pins to move the robot right or spin right
void Motor_Right(){

    P3->OUT |= AIN2;
    P3->OUT &= ~AIN1;
    P3->OUT &= ~BIN1;
    P3->OUT |= BIN2;
    Motor_GO();

    return;
}


/*  allows the user to select a specific motor function which is based on for agruements
 *   User can tell which direction to go LEFT, RIGHT, DOWN, OR UP. And on what speed and torque
 */
Motor *Motor_Input( Motor *mt, Direction direction, int freq, int duty_cycle ){

    Motor switch_bot;

    Motor_Speed(freq);
    int pos_duty = 0;
    mt->ls.L = mt->direction;

    static int old_duty =  0;

    //if duty cycle is new than update timer
    if( old_duty != duty_cycle){
        //lcd update else continue
        //printf frequency to LCD
        LCD_UNSIGNED_DEC_PRINT(COL8,ROW3,duty_cycle);
        old_duty = duty_cycle;

    }
    switch(direction){
    case FORWARD:
        //well set motors to forward and set duty cycle at desired request
        Motor_FWD();
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case BACKWARD:
        Motor_Rev();
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case LEFT:
        Motor_FWD();
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(50,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case RIGHT:
        Motor_FWD();
        pos_duty = Set_Duty(50,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case SPIN_LEFT:
        Motor_Left();
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(20,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case SPIN_RIGHT:
        Motor_Right();
        pos_duty = Set_Duty(20,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");;
        break;

    case ROTATE_LEFT:
        Motor_Left();
        pos_duty = Set_Duty(100,RIGHT_SIDE);
        pos_duty = Set_Duty(100,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case ROTATE_RIGHT:
        Motor_Right();
        pos_duty = Set_Duty(100,RIGHT_SIDE);
        pos_duty = Set_Duty(100,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case STOP:
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_Stop();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS OFF");
        break;

    case START:
        pos_duty = Set_Duty(duty_cycle,RIGHT_SIDE);
        pos_duty = Set_Duty(duty_cycle,LEFT_SIDE);
        Motor_GO();
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("WHEELS ON ");
        break;

    case LEFT_SENSOR:
        Motor_Stop();
        LCD_Clear_ROW(ROW1);
        LCD_goto_xy(COL1,ROW5);
        LCD_print_str("LEFT_SENSOR ");
        /* Code for Competition */
        Motor_Input(&switch_bot,STOP,30,100);
        Motor_Input(&switch_bot,BACKWARD,60,100);
        __delay_cycles(12500000); //0.5 sec
        Motor_Input(&switch_bot,ROTATE_LEFT,60,100);
        __delay_cycles(10000000);
        Motor_Input(&switch_bot,STOP,30,100);

        /*CODE FOR LABEXAM2 */
        //Motor_Input(&switch_bot,ROTATE_LEFT,30,100);
        //turn_rev;
        break;

    case RIGHT_SENSOR:
        Motor_Stop();
        LCD_Clear_ROW(ROW1);
        LCD_goto_xy(COL2,ROW5);
        LCD_print_str("RIGHT_SENSOR");

        /* code for competition */
        Motor_Input(&switch_bot,STOP,30,100);
        Motor_Input(&switch_bot,BACKWARD,60,100);
        __delay_cycles(12500000); //0.5 sec
        Motor_Input(&switch_bot,ROTATE_RIGHT,60,100);
        __delay_cycles(10000000);
        Motor_Input(&switch_bot,STOP,60,100);


        /*CODE FOR LABEXAM2 */
        //Motor_Input(&switch_bot,ROTATE_RIGHT,30,100);
        //turn_rev;
        break;

    case BOTH_SENSOR:
        Motor_Stop();
        LCD_Clear_ROW(ROW1);
        LCD_goto_xy(COL2,ROW1);
        LCD_print_str("BOTH_SENSOR ");

        /* code for competion */
        Motor_Input(&switch_bot,STOP,30,100);
        Motor_Input(&switch_bot,BACKWARD,60,100);
        __delay_cycles(15000000); //0.5 sec
        Motor_Input(&switch_bot,ROTATE_RIGHT,60,100);
        __delay_cycles(10000000);
        Motor_Input(&switch_bot,STOP,60,100);

        /*CODE FOR LABEXAM2 */
        //Motor_Input(&switch_bot,ROTATE_RIGHT,30,100);
        //turn_rev;

        break;

    case NO_SENSOR:
        Motor_GO();
        break;

    case UH_OH:
        Motor_Stop();
        Motor_Input(&switch_bot,SPIN_RIGHT,60,100);
        __delay_cycles(8000000); //0.5 sec
        Motor_Input(&switch_bot,ROTATE_RIGHT,60,100);
        __delay_cycles(11250000);
        break;

    }//swtich case

    mt->cs.C = direction;
    mt->direction = direction;
    mt->duty_cycle = (int)pos_duty;
    mt->freq = (int)freq;

    //printf direction based on mt cs.c current state;
    if(direction == LEFT || direction == SPIN_LEFT){
        LCD_goto_xy(COL11,ROW2);
        LCD_print_str("NW");
    }
    else if(direction == RIGHT || direction == SPIN_RIGHT){
        LCD_goto_xy(COL11,ROW2);
        LCD_print_str("NE");
    }
    else if(direction == BACKWARD){
        LCD_goto_xy(COL11,ROW2);
        LCD_print_str("S ");
    }
    else if(direction == FORWARD){
        LCD_goto_xy(COL11,ROW2);
        LCD_print_str("N ");
    }
    else{
        ;
    }

    return mt;
}


/*
 *Gives the User the current state of there motors, this will  be used in the finite state machine
 */
CURRENT_STATE Motor_Current(Motor *mt){
    return mt->cs;
}


/*
 * Gives the user the previous state of there motors
 */
LAST_STATE Motor_Previous(Motor *mt){
    return mt->ls;
}


//returns the current frequency of both PWMA and PWMB signals
int Frequency(){
    unsigned short result = (4*F_sys)/TIMER_A0->CCR[0];
    return result;
}


/*
 * Sets the Speed of the motors by changing the frequency of the PWM Wave
 * Rotational speed of a DC motor can be moduled using this equation
 * S = k2(V-R*I)/ theta
 * we can dictate the speed of the motor by changing the Voltage or our voltage
 * IE; changing the frequency of the PWM Wave
 * We will have to figure out the voltage of our PWM WAve signal by detecting its frequnecy
 */
void Motor_Speed(int speed){
    //speed will be from 0-100
    //with 100 at 100Khz all the way down to 1Khz
    // 100.47kHz = 904/9 or ccr = 904/speed
    speed = overflow(speed);
    TIMER_A0->CCR[0] = (4*F_sys)/speed;
    unsigned short freq = Frequency();

    //printf frequency to LCD
    LCD_UNSIGNED_DEC_PRINT(COL8,ROW4,freq);

    return;
}


/*
 * set the Torque of the motors by changing the duty cycle of the PWM Wave
 * The equation set to Model this value can be dervived by using this equation:
 * T = K_1 *I * theta
 * We can change the torque by chance the Current IE the duty cycle of our circuit
 *  Where the Where the Duty Cycle is set using
 *  duty = toa/T *100%
 *  T = (N * (TOP + 1))/ fsys = (N/fsys)(TOP + 1)
 *
 */
int Set_Duty(int duty, int loc){
    //Duty 0-100
    //will need to grab current CCR[0] register value
    //and also grab the its current duty cycle also


    //current CCR[0] value:
    int current_val =  TIMER_A0->CCR[0];
    duty = overflow(duty);
    //which direction
    if(loc == RIGHT_SIDE){
        //pwmA which is P2.4 or PWMA
        float new_val = (float)(current_val*(duty))/100;
        TIMER_A0->CCR[1] = rounding(new_val);
        new_val = (float)(100*new_val)/current_val;
        //printf("Duty Cycle : %f\n", new_val);
        return new_val;
    }
    if(loc == LEFT_SIDE){
        //pwmA which is P2.5 or PWMB
        float new_val = (float)(current_val*(duty))/100;
        TIMER_A0->CCR[2] = rounding(new_val);
        new_val = (float)(100*new_val)/current_val;
        //printf("Duty Cycle : %f\n", new_val);
        return new_val;
    }
    return duty;
}


/*
 * Sets the Interrupt for the TA0-Handler for motors functions
 *
 */
/*void TA0_N_IRQHandler(void){
    // better version
    //switched the TIMER_Ax from 2 to 0 from orginal code

    int tmp;
    tmp = TIMER_A0->IV;
    //flag for CCR

    if(tmp == 0x02){
        // at switch point - output 0
        TIMER_A0->CCTL[1] &= ~PWMA;
    }
    else{
        // output 1
        TIMER_A0->CCTL[2] &= ~PWMB;
    }

    return;
}
 */


//initalize PWMA for Motor
void PWMA_init(){
    //set DIR High
    P2->DIR |= PWMA;
    //Set SEL0 High And SEL1 LOW
    P2->SEL0 |= PWMA;
    P2->SEL1 &= ~PWMA;
    //Set OUT High
    P2->OUT |= PWMA;

    return;
}


//initalize PWMB for motor
void PWMB_init(){
    //set DIR High
    P2->DIR |= PWMB;
    //Set SEL0 High And SEL1 LOW
    P2->SEL0 |= PWMB;
    P2->SEL1 &= ~PWMB;
    //Set OUT High
    P2->OUT |= BIT5;

    return;
}


//sets the timers up in continous mode
void TIMER_SETUP(){
    //  SMLK || Up_mode || input divider by 1
    TIMER_A0->CTL |= (TIMER_A_CTL_SSEL__SMCLK) | TIMER_A_CTL_ID_0 | TIMER_A_CTL_MC__UP;


    TIMER_A0->CCTL[1] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set

    TIMER_A0->CCTL[2] |= (TIMER_A_CCTLN_OUTMOD_7); //reset/set

    TIMER_A0->EX0 |= 0x02; //divide by 3

    //3 MHZ /3 = 750,000 input frequency
    // CTL Frequency = 100K
    TIMER_A0->CCR[0] = 36;

    //set all motor to zero in order to have them off
    TIMER_A0->CCR[1] = 0;

    TIMER_A0->CCR[2] = 0;

    //NVIC->IP[13] |= 0x40; // Set a priority
    // ISER1 starts at 0
    //NVIC->ISER[0] |= (1<<9);

    TIMER_A0->CCR[1] = 32; //480Mhz = 32 generic 8

    TIMER_A0->CCR[2] = 20; //480MHz = 20 generic 5

    return;
}


//overflow logic checks user inputs for freq and duty cycle
int overflow(int num){
    if(num > 100){
        //printf("Integer is over 100. New int: %d\n", num-100);
        return num-100;
    }
    if(num < 0){
        int new_int = num*-1;
        if(num < -100){
            new_int = new_int-100;
            //printf("Integer <-100. New int: %d\n", new_int);
            return new_int;
        }
        //printf("Integer <0. New int: %d\n", new_int);
        return new_int;
    }
    return num;
}
