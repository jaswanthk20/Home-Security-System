/*
 * Matrix_KeyPad_JF4.c
 *
 * Created: 10/2/2022 1:38:45 PM
 * Author : sara.stout-grandy
 *
 * Read a password from the Keypad
 * Fill in the missing code at every "xx"
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "uart_utility_functions.h"

//Volatile Variable
volatile int row;
volatile int col;


//Function Prototypes
void init_hardware(void);
void set_row_low(int row);
int col_pushed(void);
char get_button(void);
char get_new_button(void);

int main(void)
{
	char b=0;
	init_uart();
	//char last_button=0;
	char pin[10]; //string as an array of chars
	char password[]="1234"; 
	int i=0;
	
	init_hardware();

	printf("Enter a 4 digit PIN (# to backup, * to erase whole thing): ");


	while (1) 
    {
		b=get_new_button();
		
		//Clear a digit 
		if (b== '#') {
			//use the backspace character to backspace the replace the character with a backspace
			if (i > 0) {
				i--;
				pin[i] = '\0';
				printf("\b \b"); 
			} //use the backspace character to backspace the replace the character with a backspace
			
			continue;//breaks one iteration
		}
		
		//Clear all digits
		if (b== '*')
		{
			//use the backspace character to backspace the replace the character with a backspace
			if (i > 0) {
						i = 0;
						pin[0] = '\0';
						printf("\nEnter a 4 digit PIN (# to backup, * to erase whole thing): ", "\r%s    \r");
			}
			
			continue;//breaks one iteration
		}

		if (b) {
			pin[i] = b;
			printf("%c",b);
			i++; //Store value pin array to "b" 
		}
		
		if (i>=4)
		{
			pin[4]='\0'; //Terminate the string with a null terminator...that makes it a string.
			if (strcmp(pin,password)) //need to use string compare!
			{
				printf("\nPIN Incorrect, try again\n"); //Print "PIN Incorrect, try again" to Putty;
				printf("Enter a 4 digit PIN (# to backup, * to erase whole thing): "); //Re-ask for password entry
				i=0;
			}
			else
			{
				printf("\nPIN Correct\n"); //Print "PIN Correct" to Putty
				break;
			}
		}
	}
	return 0;
}


char get_button(void)
{
	int key_pressed=0;
	char b;
	char buttons[4][3]={{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};//Link button push to a button label
	
	//Check for button push
	//Cycle through the rows with the for loop
	for (row=0; row < 4; row++) {
		set_row_low (row);
		_delay_ms(20);
		
		col=col_pushed();

		if (col) {
			b=buttons[row][col-1]; //b= look-up table in matrix
			_delay_ms(500);
			key_pressed=1; //A key was pressed
		}
	}
	
	if (key_pressed) {
		return b;
	}
	else {
		return 0;
	}
}

char get_new_button(void)
{
	static char last_button=0;
	char b;
	
	b=get_button(); //Call get_button function
	
	//Check if we held button down, if yes, return 0
	if (b == last_button) {
		return 0;
	}

	last_button=b;
	return b;
}

void init_hardware(void)
{
		//Set pins: rows as output and columns as inputs
		//Rows as outputs
		DDRB = 1;
		
		//Set outputs to be low
		PORTB &= ~(1<<PORTB3);
		PORTB &= ~(1<<PORTB2);
		PORTB &= ~(1<<PORTB1);
		PORTB &= ~(1<<PORTB0);
		
		//Set Columns as inputs
		DDRD = 0;
		
		//Enable pull-up resistors on the inputs
		PORTD |= (1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5);
}

void set_row_low(int row)
{
	//Hi-Z the rows (make inputs without pull-ups)
	DDRB &=~(1<< PORTB0 | 1<<PORTB1 | 1<<PORTB2 | 1<<PORTB3);
	
	//Drive the specified row low
	switch(row)
	{
		case 0: //set Row 1 low
		DDRB |= (1<<PORTB3);
		PORTB &= ~(1<<PORTB3);
		PORTB |= (1<<PORTB2);
		PORTB |= (1<<PORTB1);
		PORTB |= (1<<PORTB0);
		break;
		case 1: //set Row 2 low
		DDRB |= (1<<PORTB2);
		PORTB &= ~(1<<PORTB2);
		PORTB |= (1<<PORTB3);
		PORTB |= (1<<PORTB1);
		PORTB |= (1<<PORTB0);
		DDRB &= ~(1<<PORTB3);
		break;
		case 2: //set Row 3 low
		DDRB |= (1<<PORTB1);
		PORTB &= ~(1<<PORTB1);
		PORTB |= (1<<PORTB2);
		PORTB |= (1<<PORTB3);
		PORTB |= (1<<PORTB0);
		DDRB &= ~(1<<PORTB2);
		break;
		case 3: //set Row 4 low
		DDRB |= (1<<PORTB0);
		PORTB &= ~(1<<PORTB0);
		PORTB |= (1<<PORTB2);
		PORTB |= (1<<PORTB1);
		PORTB |= (1<<PORTB3);
		DDRB &= ~(1<<PORTB1);
		break;
		default: printf("no row set\n");
	}
}

int col_pushed(void)
{
    //Return the column that was detected

    if ((PIND & (1<<PIND7))==0) //check column 1
    {
	    return 1;
    }
    else if ((PIND & (1<<PIND6))==0) //check column 2
    {
	    return 2;
    }
    else if ((PIND & (1<<PIND5))==0) //check column 3
    {
	    return 3;
    }
    else
    {
	    return 0;
    }
    	
}
