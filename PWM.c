/*
This code generate a square wave of 50HZ at 50% 
duty cycle on GPIO2 and GPIO3 (phase shifted at 
180 degrees from each other)
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pwm.c"

#define GPIO_PF0_M1PWM4         0x00050005
#define GPIO_PF1_M1PWM5         0x00050405
#define GPIO_PF2_M1PWM6         0x00050805
#define GPIO_PF3_M1PWM7         0x00050C05

int main()
{	
    SysCtlClockSet(SYSCTL_SYSDIV_32|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // Enable the peripherals used by this program.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins

    //Configure PF1 Pins as PWM
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3);

    //Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
    //PWM_GEN_3 Covers M1PWM6 and M1PWM7 
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); 
	
    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 1626);  //4150 60 hz

    //Set PWM duty-50% (Period /2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,813); 
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,813); 

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);  // output a wave on pin (PWM1_BASE = MODULE 1, PWM_OUT_6 = PF2 (CHECK SCOPE))
    PWMOutputInvert(PWM1_BASE, PWM_OUT_7_BIT, true); // Invert 
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);  // Enable Interted Output
		
    while(1) {}    
}
