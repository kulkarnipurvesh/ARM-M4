/*
Blinky the leds using <<
*/

#include "TM4C123GH6PM.h"
#define milliseconds 1000 // ms

void delayMs(int n);

int main(void)
{
	SYSCTL->RCGCGPIO |= 0X20; 
	GPIOF->DIR= 0X0E;
	GPIOF->DEN= 0X0E;
	
	while(1)
	{
		//GPIOF->DATA= 0X0E; //Can also be used 								
		//GPIOF->DATA= 1<<3														
		//GPIOF->DATA= 1<<3 | 1<<2; //BLUE and Green both on					
		//GPIOF->DATA= 1<<3 | 1<<2 | 1<<1; // All leds on							
		GPIOF->DATA |= (0X08 | 0X02 |0x04);										
		
		delayMs(milliseconds);
		
		//GPIOF->DATA= 0;														
		//GPIOF->DATA= 0<<3														
		//GPIOF->DATA= 0<<3 | 0<<2;												
		//GPIOF->DATA= 0<<3 | 0<<2 | 0<<1;											
		GPIOF->DATA &= ~(0X08 |0X02 |0x04);									    
		delayMs(milliseconds);
	}
}

void delayMs(int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8000;j++)
			{}		
}
