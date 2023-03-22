/* FULL INTEGRATION PROJECT X.c
 *
 * Created: 22/03/2023 16:01:06
 * Authors : GROUP 3
 */ 

#include <avr/io.h>

char get_button(void);
char get_new_button(void);
void init_hardware(void);
void set_row_low(int row);
int col_pushed(void);

int main() {
    // Configure LED pins as outputs
    DDRC |= 0x0F;

    init_hardware();

    while(1) {
        // Read keypad input
        char key = get_new_button();
        if (key) {
            // Implement switch case for keypad input
            switch (key) {
                case '1':
                    PORTC |= 0x01; // turn on LED 1
                    break;
                case '2':
                    PORTC |= 0x02; // turn on LED 2
                    break;
                case '3':
                    PORTC |= 0x04; // turn on LED 3
                    break;
                case '*':
                    PORTC &= ~0x0F; // turn off all LEDs
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
    for (int row=0; row < 4; row++) {
        // Set the current row low
        set_row_low(row);
        // Check which column is pushed
        int col = col_pushed();
        if (col != -1) {
            key_pressed = 1;
            b = buttons[row][col];
            break;
        }
    }
    if (key_pressed) {
        return b;
    }
    return 0;
}

char get_new_button(void)
{
    static char old_button = 0;
    char new_button = get_button();
    if (new_button != old_button) {
        old_button = new_button;
        return new_button;
    }
    return 0;
}

void init_hardware(void)
{
    // Configure keypad rows as outputs
    DDRB |= 0x0F;
    // Configure keypad columns as inputs with pull-up resistors
    DDRD &= ~0x70;
    PORTD |= 0x70;
}

void set_row_low(int row)
{
    // Set all rows high
    PORTB |= 0x0F;
    // Set the specified row low
    PORTB &= ~(1 << row);
}

int col_pushed(void)
{
	// Check which column is pushed
	if ((PIND & (1 << PD4)) == 0) {
		return 0;
	}
	if ((PIND & (1 << PD5)) == 0) {
		return 1;
	}
	if ((PIND & (1 << PD6)) == 0) {
		return 2;
	}
	return -1;
}