/*
 * FULL INTEGRATION PROJECT X.c
 *
 * Created: 22/03/2023 16:01:06
 * Authors : GROUP 3
 */ 

#include <avr/io.h>

int main() {
	// Configure keypad pins as inputs
	DDRD &= ~0x0F;
	// Enable internal pull-up resistors for keypad pins
	PORTD |= 0x0F;

	// Configure LED pins as outputs
	DDRC |= 0x0F;

	while(1) {
		// Read keypad input
		uint8_t key = 0;
		if (!(PIND & (1 << PD0))) key = 1;
		else if (!(PIND & (1 << PD1))) key = 2;
		else if (!(PIND & (1 << PD2))) key = 3;
		else if (!(PIND & (1 << PD3))) key = 10;
		else continue; // no key pressed, wait for input

		// Implement switch case for keypad input
		switch (key) {
			case 1:
			PORTC |= 0x01; // turn on LED 1
			break;
			case 2:
			PORTC |= 0x02; // turn on LED 2
			break;
			case 3:
			PORTC |= 0x04; // turn on LED 3
			break;
			case 10:
			PORTC &= ~0x0F; // turn off all LEDs
			break;
		}
	}
	return 0;
}
