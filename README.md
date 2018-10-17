# ECEN2240_project_interrupts
A CCS project that provides interrupts for timing, etc to run TI RSLK
It is intended to be a base upon which more advanced code can be written to control
the robot car included in the kit.

This code is being developed specifically for the ECEN2440 Applications of Embedded Systems
Course at CU Boulder.


How to use this code:

(It's not ready to use yet)

Hardware setup:

Summary of pin connections:

Pin|Port   1          2                             3     4             5

0          Red LED    Left Motor Sleep              -     Bump Switch   -

1          -          Right Motor Sleep             -     Bump Switch   -

2          -          Left Motor Direction          -     Bump Switch   -

3          -          Right Motor Direction         -     Bump Switch   -

4          -          Left Motor PWM (TA0 CCR1)     -     Bump Switch   -

5          -          Right Motor PWM (TA0 CCR2)    -     Bump Switch   -

6          -          -                             -     -             -

7          -          -                             -     -             -
