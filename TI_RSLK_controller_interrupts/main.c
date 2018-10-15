/*
 *
 * Zachary Hayden
 *
 *
 */


#include "msp.h"


int calibration[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//calibration data for the line follower sensor
int reflect[8];
//reflectance data for the line follower sensor
int PWMleft0, PWMright0;
//default PWM values for the motors
int PWMleft, PWMright;
//current PWM values for the motors

//clock system control

//Timer A0: PWM generation
//port 2: motor control
//Timer A1: Line follower capture
//port 7: line follower
//Timer A2: Line follower capture
//port 5, 6: line follower
//Timer A3: general timing (1kHz)

/*Use of port  2:                           5               6
       * 0     left motor sleep
       * 1     right motor sleep
       * 2     left motor direction
       * 3     right motor direction
       * 4     left motor PWM (TA0 CCR1)
       * 5     right motor PWM (TA0 CCR2)
       * 6     unused
       * 7     unused
       *
       *
       *0      7:
       *1
       *2
       *3
       *4
       *5
       *6
       *7
       */

//-----------Interrupt handlers--------------------------------------------







//-----------Initialization code-------------------------------------------

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

       //configure Timer A 0
       TIMER_A0->CCTL[0] = 0x0080;    // CCI0 toggle
       TIMER_A0->CCR[0] =  1000;    // Period or about 10 kHz
       TIMER_A0->EX0 =     0x0005;    // Divide by 3
       TIMER_A0->CCTL[1] = 0x0040;    // CCR1 toggle/reset
       TIMER_A0->CCTL[2] = 0x0040;
       TIMER_A0->CCR[1] =  500;    // CCR1 duty cycle is 50%
       TIMER_A0->CCR[2] =  500;
       TIMER_A0->CTL =     0x0250;    // up mode, divide by 1

       //configure GPIO pins
       P2DIR = 0x3F;    //first 6 pins in port are outputs
       P2SEL0 = 0x30;   //pins 4 and 5 are PWM from TA0
       P2SEL1 &= ~0x3F;
       P2DS   = 0x3F;
       P2OUT &= 0x0F;

       //configure Timer A3
       TIMER_A3->CCTL[0] = 0x0080;    // CCI0 toggle
       TIMER_A3->CCR[0] =  10000;    // Period or about 100 Hz
       TIMER_A3->EX0 =     0x0002;    // Divide by 3
       TIMER_A3->CCTL[1] = 0x0040;    // CCR1 toggle/reset
       //TIMER_A3->CCTL[2] = 0x0040;
       TIMER_A3->CCR[1] =  200;         //after 200 counts, GPIO->input
       //TIMER_A3->CCR[2] =  500;
       TIMER_A3->CTL =     0x0210;    // up mode, divide by 1


       //infinite loop
       while(1){}
}
