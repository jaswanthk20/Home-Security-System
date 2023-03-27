/*
 * 
 * Author : nader
 *
 * Code to drive the LCD screen
 * Fill in the missing code everywhere there is an "xx"
 *
 *
 */ 

//MATRIX KEYPAD CODE IS ASSIGNMENT 5

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RS PORTC2
#define CE PORTC4

#define DB4 PORTD4
#define DB5 PORTD5
#define DB6 PORTD6
#define DB7 PORTD7

#define HALL PORTB2

#define GREEN PORTB5
#define YELLOW PORTD2
#define RED PORTC3

//D13 = GREEN
//D2 = RED
//A3 = YELLOW/BLUE

//COLUMN1 =
//COLUMN2 =
//COLUMN3 =
//COLUMN4 = 
//ROW1 = 
//ROW2 = 
//ROW3 = 

void LCD_init (void);
void LCD_command (char command);
void LCD_command_4bit (char command);
void LCD_Char (char AsciiChar);
void LCD_Send_A_String(char *StringOfCharacters);
void LCD_clearScreen (void);
void LCD_home(void);
void LCD_display(void);
void LCD_noDisplay(void);

void init_hall_LED(void);

//STATES
void armed(void);
void disarmed(void);
void triggered(void);

int main(void)
{
	int state = 0;//integer that decides what the state of the system is: 1 is armed, 0 is disarmed, 2 is intruder detected
	DDRC |= (1<<CE)|(1<<RS); //Set the display control pins as outputs (PC2, PC3, PC4)

	DDRD |= (1<<DB4)|(1<<DB5)|(1<<DB6)|(1<<DB7); //Set the parallel data pins as outputs (PD4, PD5, PD6, PD7)
	
	//set data pin outputs to low to start
	PORTC = ~(1<<RS) & ~(1<<CE);

	LCD_init();
	
	LCD_command(0x0E); //Display on, cursor on (0x0C for display ON, cursor Off)
	//LCD_command(0x1c); //Increment address by 1 and shift the cursor to the right at the time of write
	
	init_hall_LED();
	
	while(1){
		if((PINB &(1<<HALL))==0){
			state = 1;
			armed();
		}else{
			state = 2;
			triggered();
		}
	}
	
	
	//if(state == 1){
		//LCD_Send_A_String("State: armed");
		//int n = 0;
		//
		//PORTD |= (1<<YELLOW);
		//
		//while (n < 28)
		//{
			//LCD_command(0x14);
			//n++;
		//};
		//LCD_Send_A_String("*=menu, #=pin");
	//}else if(state == 0){
		//LCD_Send_A_String("State: disarmed");
		//int n = 0;
		//
		//PORTB |= (1<<GREEN);
		//
		//while (n < 25)
		//{
			//LCD_command(0x14);
			//n++;
		//};
		//LCD_Send_A_String("*=menu, #=pin");
	//}else{
		//LCD_Send_A_String("State: !!ALERT!!");
		//int n = 0;
		//
		//PORTC |= (1<<RED);
		//
		//while (n < 24)
		//{
			//LCD_command(0x14);
			//n++;
		//};
		//LCD_Send_A_String("*=menu, #=pin");
	//}
}

void LCD_init(void)
{
	_delay_ms(40); //LCD power on delay, needs to be greater than 15ms
	
	//Manual 4 bit initialization of LCD, not likely required, but doesn't harm to do it
	LCD_command_4bit(0x3);
	_delay_ms(5); //min 4.1ms
	LCD_command_4bit(0x3);
	_delay_ms(1);
	LCD_command_4bit(0x3);
	_delay_ms(1); //min of 100us
	
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0    <== See table in Datasheet, * indicated not used
	//0    0   1   0  0*   0*   0*  0*
	
	LCD_command_4bit(0x2); //Function set to 4 bit

	LCD_command(0x28); //2 line, 5*8 dots, 4 bit mode
	LCD_command(0x08); //Display off, cursor off (0x0C for display ON, cursor Off)
	LCD_command(0x01); //Display clear
	LCD_command(0x06); //Entry Set mode, increment cursor, no shift
	
	_delay_ms(2);
}
void LCD_command (char command)
{
	//Basic function used in giving commands to the LCD
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

void LCD_command_4bit (char command)
{
	//Basic function used in giving commands to the LCD
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

void LCD_Char (char AsciiChar)
{
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
void LCD_Send_A_String(char *StringOfCharacters)
{
	//Take a string input and displays it
	//Each character in the string is processed using LCD_Char which converts the character into the proper 8bit hex #
	//Max character in a string should be <16, after 16th character, everything will be ignored

	int i;
	for (i=0;StringOfCharacters[i]!=0;i++) //Send each character of string until the Null
	{
		LCD_Char(StringOfCharacters[i]);
	}
		
}

void LCD_clearScreen (void)
{
	//Clears the screen 
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	//0    0   0   0  0    0   0  1
	LCD_command(0X01); //Clear display
	_delay_ms(2);
	
	//Returns the cursor to (0,0) position
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	//0    0   0   0  1    0   0  0	
	LCD_command(0X08); //cursor at home position
}

void LCD_home(void)
{
	//Returns the cursor to (0,0) position
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	//0    0   0   0  0    0   1  0	
	LCD_command(0x02); 
}

void LCD_display(void)
{
	//Display ON with cursor OFF
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	//0    0   0   0  1    1   0  0
	LCD_command(0x0C);
}
void LCD_noDisplay(void)
{
	//Display off
	//DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	//0    0   0   0  1    0   0  0
	LCD_command(0x08);
}

void init_hall_LED(void){
	DDRB &= ~(1<<HALL);
	PORTB |=(HALL);
	
	DDRD |=(1<<YELLOW);
	DDRC |=(1<<RED);
	DDRB |=(1<<GREEN);
	
	PORTD &= ~(1<<YELLOW);
	PORTC &= ~(1<<RED);
	PORTB &= ~(1<<GREEN);
}

void armed(void){
	LCD_Send_A_String("State: armed");
	int n = 0;
	
	PORTD |= (1<<YELLOW);
	PORTC &= ~(1<<RED);
	PORTB &= ~(1<<GREEN);
	
	while (n < 28)
	{
		LCD_command(0x14);
		n++;
	};
	LCD_Send_A_String("*=menu, #=pin");
	
	break;
}

void disarmed(void);


void triggered(void){
	LCD_Send_A_String("State: !!ALERT!!");
	int n = 0;
	
	PORTC |= (1<<RED);
	PORTD &= ~(1<<YELLOW);
	PORTB &= ~(1<<GREEN);
	
	while (n < 24)
	{
		LCD_command(0x14);
		n++;
	};
	LCD_Send_A_String("*=menu, #=pin");
	
	break;
}
