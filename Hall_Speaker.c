//Aarons code for hall effect sensor, led and speaker unit

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "uart.h"

void init_hardware(void);
void init_PAM8302A(void);
void init_hall_sensor(void);
void init_LED(void);

void trigger_event(void);

int main(void)
{
	init_hardware();

	//Initialize UART for debugging output
	init_uart();
	printf("System Booted, built %s on %s\n ", __TIME__,__DATE__);
	printf("Bring a Magnet Close to the Hall Sensor, see LED come on!\n");

	while (1)
	{
		// Trigger event based on input from other sensors
		trigger_event();
	}
}

void init_hardware(void)
{
	// Set up PAM8302A shutdown pin
	DDRD |= (1<<PD3); // Set PD3 as output
	PORTD |= (1<<PD3); // Set PD3 high initially

	// Call functions to init components
	init_PAM8302A();
	init_hall_sensor();
	init_LED();
}

void init_PAM8302A(void)
{
	// Set up PAM8302A
	DDRD |= (1<<PD4); // Set PD4 as output 
	PORTD &= ~(1<<PD4); // Set PD4 low 
}

void init_hall_sensor(void)
{
	// Set up hall sensor
	DDRB &= ~(1<<PB0); // Set PB0 as input from sensor
	PORTB |= (1<<PB0); // Enable pull-up resistor on input
}

void init_LED(void)
{
	// Set up LED
	DDRD |= (1<<PD7); // Set PD7 as output to LED
	PORTD &= ~(1<<PD7); // Set PD7 low initially
}

void trigger_event(void)
{
	// Read the hall sensor
	if ((PINB & (1<<PB0))==0)
	{
		// Magnet detected, drive the LED high to turn it on
		PORTD |= (1<<PD7); // Drive LED High
		printf("Magnet Detected!\n");
		
		// Play audio through PAM8302A
		PORTD |= (1<<PD4); // Set PD4 high to enable amplifier
		_delay_ms(500);
		PORTD &= ~(1<<PD4); // Set PD4 low to disable amplifier
	}
	else
	{
		// No magnet detected, drive the LED low to turn it off
		PORTD &= ~(1<<PD7); // Drive LED low
		//printf("No Magnet\n");
	}
}
