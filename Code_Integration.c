//Author: Nader Hdeib B00898627

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

//volatile variables for column and row detection on keypad
volatile int row;
volatile int col;

//keypad row definitions
#define ROW1 PORTC0
#define ROW2 PORTC1
#define ROW3 PORTB4
#define ROW4 PORTB3

//keypad column definitions
#define COL1 PORTB2
#define COL2 PORTB1
#define COL3 PORTB0

//LCD command pins definitions
#define RS PORTC2
#define CE PORTC4

//LCD data pins definitions
#define DB4 PORTD4
#define DB5 PORTD5
#define DB6 PORTD6
#define DB7 PORTD7

//sensor input pins definitions
#define HALL PORTC5
#define HALL2 PORTD0

//LED and speaker pins definitions
#define GREEN PORTB5
#define YELLOW PORTD2
#define RED PORTC3
#define SPEAKER PD3

//keypad functions
void init_keypad(void);
void set_row_low(int row);
int col_pushed(void);
char get_button(void);
char get_new_button(void);

//sensors speaker and LED initialization
void init_Sense_LED(void);

//LCD functions
void LCD_init (void);
void LCD_command (char command);
void LCD_command_4bit (char command);
void LCD_Char (char AsciiChar);
void LCD_Send_A_String(char *StringOfCharacters);
void LCD_clearScreen (void);
void LCD_home(void);
void LCD_display(void);
void LCD_noDisplay(void);

//state functions
void armed(void);
void disarmed(void);
void triggered(void);
void brute_force(void);

int state = 1;
char b=0;
char pin[10];

//built in password
char password[]="4064";
int i=0;

int main(void){
	//initiates hardware
	init_keypad();
	LCD_init();
	init_Sense_LED();
	
	//main loop that can be returned to from other loops via break statements under specific conditions
	//contains switch statement that depends on int: state that can be set to 1, 2, or 3, depending on the state of the system.
	while(1){
		switch (state){
			case 1:
			disarmed();
			break;
			case 2:
			armed();
			break;
			case 3:
			triggered();
			break;
			default:
			LCD_Send_A_String("Reboot Hardware");
		}
	}
	return 0;
}

//keypad funcs
char get_button(void){
	int key_pressed=0;
	char b;
	char buttons[4][3]={{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}}; 

	for (row = 0 ; row < 4 ; row++){
		set_row_low (row);
		_delay_ms(20);
		
		col=col_pushed();
		
		if (col){
			b=buttons[row][col-1];
			_delay_ms(500);
			key_pressed=1;
		}
	}
	
	if (key_pressed){
		return b;
	}else{
		return 0;
	}
}

char get_new_button(void){
	static char last_button=0;
	char b;	
	b=get_button();

	if (b == last_button) {
		return 0;
	}
	
	last_button=b;
	return b;
}

void init_keypad(void){
	DDRB |=(1<<ROW3)|(1<<ROW4);
	DDRC |=(1<<ROW1)|(1<<ROW2);
	//Set outputs to be low
	PORTC &= ~(1<<ROW1);
	PORTC &= ~(1<<ROW2);
	PORTB &= ~(1<<ROW3);
	PORTB &= ~(1<<ROW4);
	//Set Columns as inputs
	DDRB &= (1<<COL1 | 1<<COL2 | 1<<COL3);
	//Enable pull-up resistors on the inputs
	PORTB |= (1<<COL1) | (1<<COL2) | (1<<COL3);
}

void set_row_low(int row){
	DDRB &=~(1<< ROW3 | 1<<ROW4);
	DDRC &=~(1<< ROW1 | 1<<ROW2);
	//Drive the specified row low
	switch(row)
	{
		case 0: //set Row 1 low
		DDRC |= (1<<ROW1);
		PORTC &= ~(1<<ROW1);
		PORTC |= (1<<ROW2);
		PORTB |= (1<<ROW3);
		PORTB |= (1<<ROW4);
		break;
		case 1: //set Row 2 low
		DDRC |= (1<<ROW2);
		PORTC &= ~(1<<ROW2);
		PORTC |= (1<<ROW1);
		PORTB |= (1<<ROW3);
		PORTB |= (1<<ROW4);
		DDRC &= ~(1<<ROW1);
		break;
		case 2: //set Row 3 low
		DDRB |= (1<<ROW3);
		PORTB &= ~(1<<ROW3);
		PORTC |= (1<<ROW2);
		PORTC |= (1<<ROW1);
		PORTB |= (1<<ROW4);
		DDRC &= ~(1<<ROW2);
		break;
		case 3: //set Row 4 low
		DDRB |= (1<<ROW4);
		PORTB &= ~(1<<ROW4);
		PORTC |= (1<<ROW2);
		PORTB |= (1<<ROW3);
		PORTC |= (1<<ROW1);
		DDRB &= ~(1<<ROW3);
		break;
	}
}

int col_pushed(void){
	if ((PINB & (1<<PINB2))==0){
		return 1;
	}else if ((PINB & (1<<PINB1))==0){
		return 2;
	}else if ((PINB & (1<<PINB0))==0){
		return 3;
	}else{
		return 0;
	}
}

//lcd funcs
void LCD_init(void){
	DDRC |= (1<<CE)|(1<<RS); //Set the display control pins as outputs (PC2, PC3, PC4)
	DDRD |= (1<<DB4)|(1<<DB5)|(1<<DB6)|(1<<DB7); //Set the parallel data pins as outputs (PD4, PD5, PD6, PD7)
	PORTC &= ~(1<<RS | 1<<CE);
	_delay_ms(40);
	LCD_command_4bit(0x3);
	_delay_ms(5); //min 4.1ms
	LCD_command_4bit(0x3);
	_delay_ms(1);
	LCD_command_4bit(0x3);
	_delay_ms(1); //min of 100us
	LCD_command_4bit(0x2); //Function set to 4 bit
	LCD_command(0x28); //2 line, 5*8 dots, 4 bit mode
	LCD_command(0x08); //Display off, cursor off (0x0C for display ON, cursor Off)
	LCD_command(0x01); //Display clear
	LCD_command(0x06); //Entry Set mode, increment cursor, no shift
	_delay_ms(2);
	LCD_command(0x0E);
}

void LCD_command (char command){
	char UpperHalf, LowerHalf;
	UpperHalf=command & 0xF0;	//Take upper 4 bits of command
	PORTD &= 0x0F; //Flushes upper half of PortC to 0, but keeps lower half
	PORTD |= UpperHalf;
	PORTC &=~(1<<RS); //Clear RS for command register
	//PORTC &=~(1<<RW); //Clear RW for IR
	PORTC |= (1<<CE); //Set CE
	_delay_us(1);
	PORTC &= ~(1<<CE); //Clear CE
	_delay_us(200);
	LowerHalf=(command<<4); //Lower 4 bits of the command
	PORTD &= 0x0F; //Flushes upper half of PortC to 0, but keeps lower half
	PORTD |= LowerHalf;
	PORTC |= (1<<CE); //Set CE
	_delay_us(1);
	PORTC &= ~(1<<CE); //clear CE
	_delay_ms(2);
}

void LCD_command_4bit (char command){
	char LowerHalf;
	LowerHalf=(command<<4); //Lower 4 bits of the command
	PORTD &= 0x0F; //Flushes upper half of PortC to 0, but keeps lower half
	PORTD |= LowerHalf;
	PORTC &=~(1<<RS); //Clear RS for command register
	PORTC |= (1<<CE); //Set CE
	_delay_us(1);
	PORTC &= ~(1<<CE); //clear CE
	_delay_ms(2);
}

void LCD_Char (char AsciiChar){
	char UpperHalf, LowerHalf;
	UpperHalf=AsciiChar & 0xF0; //Upper 4 bits of data
	PORTD &= 0x0F; //Flushes upper half of PORTD to 0, but keeps lower half
	PORTD |= UpperHalf;
	PORTC |=(1<<RS); //Set RS for data register
	//PORTC &=~(1<<RW); //Clear RW for write data, Set RW to read data
	PORTC |= (1<<CE); //Set CE
	_delay_us(1);
	PORTC &= ~(1<<CE); //Clear CE
	_delay_us(200);
	LowerHalf=(AsciiChar<<4); //Lower 4 bits of the command
	PORTD &= 0x0F; //Flushes upper half of PORTD to 0, but keeps lower half
	PORTD |= LowerHalf;
	PORTC |= (1<<CE); //Set CE
	_delay_us(1);
	PORTC &= ~(1<<CE);
	_delay_ms(2);
}

void LCD_Send_A_String(char *StringOfCharacters){
	int i;
	for (i=0;StringOfCharacters[i]!=0;i++) //Send each character of string until the Null
	{
		LCD_Char(StringOfCharacters[i]);
	}
}

void LCD_clearScreen (void){
	LCD_command(0X01);
	_delay_ms(2);
	LCD_command(0X08); 
}

void LCD_home(void){
	LCD_command(0x02);
}

void LCD_display(void){
	LCD_command(0x0C);
}

void LCD_noDisplay(void){
	LCD_command(0x08);
}

//sensors, speaker and LEDs
void init_Sense_LED(void){
	
	//initializing first hall effect sensor as an input with pull up
	DDRB &= ~(1<<HALL);
	PORTB |= (1<<HALL);
	
	//initializing second hall effect sensor as an input with pull up
	DDRB &= ~(1<<HALL2);
	PORTD |=(1<<HALL2);
	
	//initializing LED pins to outputs
	DDRD |=(1<<YELLOW);
	DDRC |=(1<<RED);
	DDRB |=(1<<GREEN);
	
	//initializing LED pins to 0
	PORTD &= ~(1<<YELLOW);
	PORTC &= ~(1<<RED);
	PORTB &= ~(1<<GREEN);
	
	//initializing speaker output pin
	DDRD |= (1<<SPEAKER);
	OCR2A = 0;
	OCR2B = 15;
	TCCR2A |=(1<<COM2B1);
	TCCR2A |= (1<<WGM21) | (1<<WGM20);
	TCCR2B |= (1<<WGM22);
	TCCR2B |= (1<<CS22) | (1<< CS20);

}
	
//states
void disarmed(void){
	//initializing brute force counter
	int k = 0;
	//setting LEDs to respectively required colors for state
	PORTC &= ~(1<<RED);
	PORTD &= ~(1<<YELLOW);
	PORTB |= (1<<GREEN);
	//displaying LCD message
	LCD_command(0x01);
	LCD_Send_A_String("System Disarmed");
	LCD_command(0xC0);
	
	//beginning main disarmed function loop, which only exits if the correct password is inputted
	while (1){
		
		//check for brute force attempts
		//5th incorrect attempt launches program into infinite loop where machine can only be hardware reset
		if(k > 4){
			brute_force();
		}
		b=get_new_button();
		
		//used to backspace a single character in password entry
		if (b== '#') {
			if (i > 0) {
				i--;
				pin[i] = '\0';
			}
			LCD_command(0x10);
			_delay_ms(20);
			LCD_command(0x10);
			continue;
		}
		
		//used to clear entire password entry
		if (b== '*'){
			if (i > 0) {
				i = 0;
				pin[0] = '\0';
				
			}
			LCD_command(0x01);
			LCD_Send_A_String("System Disarmed");
			LCD_command(0xC0);
			continue;//breaks one iteration
		}
		//the condition below stores entered characters into a string to compare to the built in password
		if (b) {
			pin[i] = b;
			i++; //Store value pin array to "b"
			LCD_Char(b);
			LCD_command(0x14);
		}
		
		//the condition below terminates string at 4 characters and compares to built in password
		if (i>=4)
		{
			pin[4]='\0';
			//the below condition means the password is incorrect
			if (strcmp(pin,password)){
				i=0;
				LCD_command(0x01);
				LCD_Send_A_String("System Disarmed");
				LCD_command(0xC0);
				//brute force counter
				k ++;
			//the below condition means the password is correct, allowing the machine to switch to the next state
			}else{
				i=0;
				//set state to armed state so that the switch function in the main loop routes the program to the armed state function
				state = 2;
				break;
			}
		}
	}
}

void armed(void){
	//setting LEDs to respectively required colors for state
	PORTD |= (1<<YELLOW);
	PORTC &= ~(1<<RED);
	PORTB &= ~(1<<GREEN);
	//displaying LCD message
	LCD_command(0x01);
	LCD_Send_A_String("System armed");
	LCD_command(0xC0);
	LCD_Send_A_String("Polling intruder");
	
	//beginning main armed function loop, which only exits if the hall effect sensors no longer detect a magnet
	while(1){
		if(((PINC & (1<<PINC5))!=0) | ((PIND & (1<<HALL2)) != 0)){
			//set state to the triggered state so that the switch function in the main loop routes the program to the triggered state function
			state = 3;
			break;
		}
	}
}

void triggered(void){
	//initializing brute force counter
	int j = 0;
	//setting LEDs to respectively required colors for state
	init_Sense_LED();
	PORTC |= (1<<RED);
	PORTD &= ~(1<<YELLOW);
	PORTB &= ~(1<<GREEN);
	//displaying LCD message
	LCD_command(0x01);
	LCD_Send_A_String("Pin to disarm:");
	LCD_command(0xC0);
	
	//beginning main triggered function loop, which only exits if the correct password is inputted
	while (1){
		//check for brute force attempts
		//5th incorrect attempt launches program into infinite loop where machine can only be hardware reset
		if(j > 4){
			brute_force();
		}
		//initiate siren tone
		OCR2A =  30;
		b=get_new_button();
		
		//used to backspace a single character in password entry
		if (b== '#') {
			if (i > 0) {
				i--;
				pin[i] = '\0';
			}
			LCD_command(0x10);
			_delay_ms(20);
			LCD_command(0x10);
			continue;
			
		}
		
		//used to clear entire password entry
		if (b== '*'){
			if (i > 0) {
				i = 0;
				pin[0] = '\0';
			}
			LCD_command(0x01);
			LCD_Send_A_String("Pin to disarm:");
			LCD_command(0xC0);
			continue;//breaks one iteration
		}
		
		//the condition below stores entered characters into a string to compare to the built in password
		if (b) {
			pin[i] = b;
			i++; //Store value pin array to "b"
			LCD_Char(b);
			LCD_command(0x14);
		}
		
		//the condition below terminates string at 4 characters and compares to built in password
		if (i>=4)
		{
			pin[4]='\0';
			//the below condition means the password is incorrect
			if (strcmp(pin,password)){
				i=0;
				LCD_command(0x01);
				LCD_Send_A_String("Pin to disarm:");
				LCD_command(0xC0);
				//brute force counter
				j++;
			//the below condition means the password is correct, allowing the machine to switch to the next state
			}else{
				i=0;
				//set state to disarmed state so that the switch function in the main loop routes the program to the disarmed state function
				state = 1;
				OCR2A = 0;
				break;
			}
		}
		
	}
}

//Infinite loop function that instructs user to reset hardware in the case that a brute force attack was detected
//entering the password incorrectly 5 times is considered a brute force attack
void brute_force(void){
	init_Sense_LED();
	PORTC |= (1<<RED);
	PORTD &= ~(1<<YELLOW);
	PORTB &= ~(1<<GREEN);
	LCD_command(0x01);
	LCD_Send_A_String("Brute force");
	LCD_command(0xC0);
	LCD_Send_A_String("detected");
	_delay_ms(5000);
	LCD_command(0x01);
	LCD_Send_A_String("Reset system to");
	LCD_command(0xC0);
	LCD_Send_A_String("disarm");
	while (1){
		OCR2A =  30;
	}
}
