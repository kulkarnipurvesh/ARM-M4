#include "TM4C123GH6PM.h"
#define pin 0x0E // 0X08 GREEN, 0X04 Blue, 0x02 Red, 0x0E White

void delayMs(int n);

int main(void)
{
	SYSCTL->RCGCGPIO |= 0X20; // Turn on clock on PORTF, Check page 30 
	GPIOF->DIR= pin; // Set direction
	GPIOF->DEN= pin; // Digital Enable
	
	while(1)
	{
		GPIOF->DATA= pin; // set PF1,2,3 high
		delayMs(1000); 
		GPIOF->DATA= 0;
		delayMs(1000);
	}
}

void delayMs(int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8000;j++) // custom defined 8000
			{}		
}


