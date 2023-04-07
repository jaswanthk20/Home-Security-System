/*
 * HC-SR501_Better.c
 *
 * Created: 10/2/2022 12:42:34 PM
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    int val=0; //variable for sensor input
	int pirState=0; //To keep track of the state of the PIR sensor
	
	init_uart();
	
	DDRD |= (1<<DDD2);		// Set pin as Output to LED.
 // UCSR0B &= ~(1<<RXEN0);
	//DDRD &= ~(1<<DDD0);		// Set pin as Input for PIR Output

    while (1) {
		val= PIND & (1<<PD0); //Read the Sensor, store in variable val
		if (val) //if val is high
		{
			PORTD |= (1<<DDD2); //Turn the LED on (set the bit high)
			if (pirState == 0) //if state of PIR sensor was zero
			{
				printf("Motion Detected!\n\n");
				pirState=1;
			}
			
		}
		else {
			PORTD &= ~(1<<DDD2); //Turn the LED off (set the bit low)
			if (pirState == 1)  //if state of PIR sensor was one
			{
				printf("Motion Ended!\n\n");
				pirState=0;
			}
			
		} 
    }
}
