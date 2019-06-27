/*
This code blinks green led continuously
and also has an interrupt for switch on PF0 
which would toggle red and blue at the same time
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

void IntGlobalEnable(void)
{
 __asm("cpsie i\n"); // Inline assembly
}

void portInit(void)
{
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x01;
	
	GPIO_PORTF_DIR_R |= 0x0E; // SET PF 1,2,3 as output
	GPIO_PORTF_DIR_R &= ~0x01; // Set PF0 as input
	
	GPIO_PORTF_DEN_R |= 0x0F; // Digital enable PF 0,1,2,3
	
	GPIO_PORTF_PUR_R |= 0x01; // Pull Up PF0
}

void intInit(void)
{
	NVIC_EN0_R |=0x40000000;		// enable interrupt in NVIC 
	IntPrioritySet(INT_GPIOF,0xE0); // set priority (change upper bits only)
	
	GPIO_PORTF_IM_R |= 0x01; // set interrupt mask to enable interrupt on PORTF pin
	GPIO_PORTF_IS_R &= ~0x01; // make it edge sensitive
	GPIO_PORTF_IBE_R |= 0x01; // make both edge sensitive
	//GPIO_PORTF_IEV_R &= ~0x01; // by clearing make falling edge sensitive, by setting make rising edge sensitive
	
	IntGlobalEnable(); // globally enable interrupts (has its own assembly code defined at top)
}
int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_2|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	
	portInit();
	intInit();
	
	while(1)
	{
		GPIO_PORTF_DATA_R ^= 0x08; // blink Green LED for no reason
		SysCtlDelay(1000000);
	}
}

void GPIOF_Handler(void)
{
	GPIO_PORTF_ICR_R |=0x01; // Clear the interrupt of PF0 
	GPIO_PORTF_DATA_R ^= 0x06; // toggle Blue and Green led
}
