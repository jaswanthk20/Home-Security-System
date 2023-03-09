/*
 * LCD SCREEN.c
 *
 * Created: 2023-03-05 8:21:26 PM
 * Author : nader
 */ 


#define epin = PINC0
#define eport = PORTC
#define rspin = PINB5
#define rsport = PORTB
#define d1 = PINB4
#define d2 = PIND4
#define d3 = PIND3
#define d4 = PIND2
#define data = 

#include <avr/io.h>


void screen_cmd(unsigned char ch);

int main(void)
{
	screen_cmd(0x28);
	screen_cmd(0x0e);
    DDRB = 0xff;
	DDRD = 0xff;
	DDRC = 0xff;	
	
	/* Replace with your application code */
    while (1) 
    {
		
    }
}

void screen_cmd(int select, char *data_in){
	char data;
	
	for(int i = 0 ; data_in[i] != '\0'; i++);
	
	int length = i;
	
	for(int j = 0; j < i; j++){
		if(j==16){
			screen_cmd(0, 0xC0);
		}
		
		data = data_in[j];
		if(select == 1){
			rsport = (1<<rspin);
		}else if(select == 0){
			rsport &= ~(1<<rspin);
		}
		
		
	}
	
	
	data = ch;
	rspin = 0;
	epin = 1;
	
	//delay 10 ms
	
	epin = 0;
	
}

void screen_data(unsigned char ch){
	
}


