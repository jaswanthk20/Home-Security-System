/*
 * Audio_Shell.c
 *
 * Created: 10/9/2022 3:22:25 PM
 *
 * Audio amplifier (PAM8302) driver into Speaker to create Sound.
 *
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	//uint8_t song[] = {128,64,255,64,255,16,32};
//	uint8_t song2[] = {120, 130, 125, 135, 120, 125, 130, 135, 140, 140, 130};
	
	DDRD |= (1<<PORTD3);; //Set port pin PD3 as an output to drive audio Amp.
	
	OCR2A =180; //OCR2A set the top value of the timer/counter2 
	OCR2B =20; //OCR2B/OCR2A sets the PWM duty cycle to 50%

	TCCR2A |= (1<<COM2B1); //Set Clear OC2B on Compare Match, set OC2B at Bottom in non-inverting mode
	TCCR2A |= (1<<WGM21) | (1<<WGM20); //Set Fast PWM mode
	TCCR2B |= (1<<WGM22); //Set OCRA to represent the top count
	TCCR2B |= (1<<CS22) | (1<<CS20); //Set prescaler to 128, starts counter!

	//Play song
	//for (unsigned int i=0; i<sizeof(song); i++)
	//{
		//OCR2A = song[i]; //Set OCR2A equal to the various values from song[]
		//_delay_ms(200);
	//}
	//
		//DDRD=0; //Song over
		
	while (1) 
    {
		//Make a siren by changing the frequency
		OCR2A =30; //Set counts to get OC2B f=1.95 kHz
		_delay_ms(500);
		OCR2A=40; //Set counts to get OC2B f=976.5 Hz
		_delay_ms(500);
	
    }
	
}
