/* TI RSLK Controller (Interrupts)
 * Zachary Hayden, University of Colorado at Boulder
 *
 * The purpose of this code is to provide a basic set of ISRs
 * that provide all required information and timing to control
 * the robot car from TI's RSLK
 *
 * The target is the MSP432P401R included in the RSLK
 */


#include "msp.h"

void timing1(void){}
//void timing2(void){}
//void timing3(void){}
//void timing4(void){}

void bump(void){}

int calibration[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//calibration data for the line follower sensor
int reflect[8];
//reflectance data for the line follower sensor
int PWMleft0, PWMright0;
//default PWM values for the motors
int PWMleft, PWMright;
//current PWM values for the motors

//clock system control:
//default for MSP-EXP432P401R: 3MHz SMCLK

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
       *        7:
       *0
       *1
       *2
       *3
       *4
       *5
       *6
       *7
       */

//-----------Interrupt handlers--------------------------------------------

//timing interrupts
//timer3 reset
void TA3_0_IRQHandler(void){
    //reset interrupt flag

    //
}

//timer3 Compare
void TA3_N_IRQHandler(void){
    //reset interrupt flag + copy the TA3IV register
    short iv = TA3IV;

    switch(iv){
    case 0x02:
        timing1();
        break;
//    case 0x04:
//        timing2();
//        break;
//    case 0x06:
//        timing3();
//        break;
//    case 0x08:
//        timing4();
//        break;
    }

}

//bump switch interrupt handler
void PORT4_IRQHandler(void){
    //repeated interrupts read through P4IV
    //or
    //one ISR that detects all switch presses?
}


//-----------Initialization code-------------------------------------------

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

       //configure Timer A0 (modify!)
       TIMER_A0->CCTL[0] = 0x0080;    // CCI0 toggle
       TIMER_A0->CCR[0] =  1000;    // Period or about 10 kHz
       TIMER_A0->EX0 =     0x0005;    // Divide by 3
       TIMER_A0->CCTL[1] = 0x0040;    // CCR1 toggle/reset
       TIMER_A0->CCTL[2] = 0x0040;
       TIMER_A0->CCR[1] =  500;    // CCR1 duty cycle is 50%
       TIMER_A0->CCR[2] =  500;
       TIMER_A0->CTL =     0x0250;    // up mode, divide by 1

       //configure Timer A1 (capture 0-3)
       TIMER_A1->CCTL[0] = 0x0080;        //compare mode
       TIMER_A1->CCR[0] = 0xFFFF;    //maximum count range
       TIMER_A1->CCTL[1] = 0x8900;//capture mode (falling edge), CCI1A, SCS=1,
       TIMER_A1->CCTL[2] = 0x8900;//outmode: out bit, no interrupts
       TIMER_A1->CCTL[3] = 0x8900;
       TIMER_A1->CCTL[4] = 0x8900;
       TIMER_A1->CTL = 0x0210;//SMCLK/1(3MHz), up mode, no interrupts

       //configure Timer A2 (capture 4-7)
       TIMER_A2->CCTL[0] = 0x0080;
       TIMER_A2->CCR[0] = 0xFFFF;
       TIMER_A2->CCTL[1] = 0x8900;//capture mode (falling edge), CCI1A, SCS=1,
       TIMER_A2->CCTL[2] = 0x8900;//outmode: out bit, no interrupts
       TIMER_A2->CCTL[3] = 0x8900;
       TIMER_A2->CCTL[4] = 0x8900;
       TIMER_A2->CTL = 0x0210;

       //configure Timer A3 (general-purpose timing)
       TIMER_A3->CCTL[0] = 0x0080;    // CCI0 toggle
       TIMER_A3->CCR[0] =  1000;    // Period of about 100 Hz
       TIMER_A3->EX0 =     0x0002;    // Divide by 3
       TIMER_A3->CCTL[1] = 0x0040;    // CCR1 toggle/reset
       TIMER_A3->CCR[1] =  20;         //after 20 counts, GPIO->input
       //TIMER_A3->CCTL[2] = 0x0040;
       //TIMER_A3->CCR[2] =  500;
       TIMER_A3->CTL =     0x0210;    // up mode, divide by 1

       //configure GPIO pins
       //configure port 1 (red LED, Motor Control)

       //configure port 2 (RGB LED, PWM out)
       P2DIR = 0x3F;    //first 6 pins in port are outputs
       P2SEL0 = 0x30;   //pins 4 and 5 are PWM from TA0
       P2SEL1 &= ~0x3F;
       P2DS   = 0x3F;
       P2OUT &= 0x0F;
       //configure port 3 (velocity sensor input)

       //configure port 4 (bump switch input)

       //configure port 5 (capture input)

       //configure port 6 (capture input)

       //configure port 7 (capture input)

       //configure port 8 (unused)
       P8DIR  = 0xFF;   //all outputs
       P8SEL0 = 0x00;   //as GPIO
       P8SEL1 = 0x00;   //minimize power

       //configure port 9 (unused)
       P9DIR  = 0xFF;   //all outputs
       P9SEL0 = 0x00;   //as GPIO
       P9SEL1 = 0x00;   //minimize power

       //configure port 10 (unused)
       P10DIR  = 0xFF;   //all outputs
       P10SEL0 = 0x00;   //as GPIO
       P10SEL1 = 0x00;   //minimize power

       //enable interrupts
       NVIC_EnableIRQ(PORT4_IRQn);
       NVIC_EnableIRQ(TA3_0_IRQn);
       NVIC_EnableIRQ(TA3_N_IRQn);


//----------------infinite loop-----------------------------------
       while(1){}
}
