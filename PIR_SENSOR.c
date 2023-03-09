/*
 * HC-SR501_Better.c
 *
 * Created: 10/2/2022 12:42:34 PM
 * Author : Dr. sara.stout-grandy
 *
 * Complete the missing code everywhere there is an "xx".
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(void)
{
    int val=0; //variable for sensor input
	int pirState=0; //To keep track of the state of the PIR sensor
	
	init_uart();
	
	DDRB |= (1<<DDB0);		// Set pin as Output to LED.
	DDRD &= ~(1<<DDD2);		// Set pin as Input for PIR Output
	
	printf("System Booted, built %s on %s\n ", __TIME__,__DATE__);

	printf("PIR Sensor Testing: Move hand in front of the sensor.\n\n");

    while (1) 
    {
		val= PIND & (1<<PD2); //Read the Sensor, store in variable val
		if (val) //if val is high
		{
			PORTB |= (1<<DDB0); //Turn the LED on (set the bit high)
			if (pirState == 0) //if state of PIR sensor was zero
			{
				printf("Motion Detected!\n\n");
				pirState=1;
			}
			
		}
		else
		{
			PORTB &= ~(1<<DDB0); //Turn the LED off (set the bit low)
			if (pirState == 1)  //if state of PIR sensor was one
			{
				printf("Motion Ended!\n\n");
				pirState=0;
			}
			
		}
    }
}

