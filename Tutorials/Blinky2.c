/*
###########################################################
1.For different methods,check page 19,20
2.First activate GPIO ports by giving clock to RCGCGPIO, in accordance to page 30 (bottom)
3.Then set direction using DIR
4.Then make that port digitally enabled (not analog) using DEN
5.Method 2,3,3.1 uses mask hence wont affect other pins and would only change that particular pin, page 19,20
###########################################################
*/

#include "TM4C123GH6PM.h"
#define seescope 1000 // ms

void delayMs(int n);

int main(void)
{
	SYSCTL->RCGCGPIO |= 0X20; 
	GPIOF->DIR= 0X0E;
	GPIOF->DEN= 0X0E;
	
	while(1)
	{
		//GPIOF->DATA= 0X0E; //Can also be used 								//Method 1
		//GPIOF->DATA= 1<<3														//Method 2
		//GPIOF->DATA= 1<<3 | 1<<2; //BLUE and Green both on					//Method 3
		//GPIOF->DATA= 1<<3 | 1<<2 | 1<<1; // All leds on							//Method 3.1
		GPIOF->DATA |= (0X08 | 0X02 |0x04);										//Method 4
		
		delayMs(seescope);
		
		//GPIOF->DATA= 0;														//Method 1
		//GPIOF->DATA= 0<<3														//Method 2
		//GPIOF->DATA= 0<<3 | 0<<2;												//Method 3
		//GPIOF->DATA= 0<<3 | 0<<2 | 0<<1;											//Method 3.1
		GPIOF->DATA &= ~(0X08 |0X02 |0x04);									    //Method 4
		delayMs(seescope);
	}
}

void delayMs(int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8000;j++)
			{}		
}
