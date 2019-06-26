/*
This code takes value from PD0 (ch7or ain7) coverts using adc and displays it serial monitor.
Some lines are commented in the main section for the use of internal temperature sensor.
*/

#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include <string.h>

void InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}

int main()
	{

		SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
		InitConsole();
		uint32_t ADCValues[1];
		uint32_t TempValueC ;
		uint32_t TempValueF ;
			
		UARTprintf("ADC ->\n");
		UARTprintf("  Type: Internal Temperature Sensor\n");
		UARTprintf("  Samples: One\n");
		UARTprintf("  Update Rate: 250ms\n");
		UARTprintf("  Input Pin: Internal temperature sensor\n\n");

		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		SysCtlDelay(3);
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH7 | ADC_CTL_IE |ADC_CTL_END); //ain7 = pd0, replace CH7 to TS for internal temperature sensor
		
		ADCSequenceEnable(ADC0_BASE, 3);
		ADCIntClear(ADC0_BASE, 3);

	    while(1)
	    {
	        // Trigger the ADC conversion.
	        ADCProcessorTrigger(ADC0_BASE, 3);
	        
	        // Wait for conversion to be completed.
	        while(!ADCIntStatus(ADC0_BASE, 3, false))
	        {
	        }

	        // Clear the ADC interrupt flag.
	        ADCIntClear(ADC0_BASE, 3);
	        // Read ADC Value.
	        ADCSequenceDataGet(ADC0_BASE, 3, ADCValues);
	        
	        // TempValueC = (uint32_t)(147.5 - ((75.0*3.3 *(float)ADCValues[0])) / 4096.0); // for internal
			TempValueF = (3.3 * ADCValues[0] * 100.0)/4096.0; // for lm34
	        
	        // TempValueF = ((TempValueC * 9) + 160) / 5; // for internal
			TempValueC = (TempValueF - 32) * (5.0/9.0); // for lm34

	        // Display the temperature value on the console.
	        UARTprintf("Temperature = %3d*C or %3d*F\r", TempValueC,TempValueF);

	        SysCtlDelay(80000000 / 12);
	    }
}