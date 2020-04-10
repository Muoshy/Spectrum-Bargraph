/*
 * spectrum analyzer - attiny85
 *
 * Created: 2020-04-05 18:03:47
 * Author : Daniel Quach
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define STROBE 1
#define RESET 0

#define S0 5
#define S1 3
#define S2 4
#define ENABLE 2

int main(void)
{
	DDRB |= 1 << S0; 		//set data direction for control pins (74HC4051)
	DDRB |= 1 << S1;
	DDRB |= 1 << S2;
	DDRB |= 1 << ENABLE;

	DDRB |= 1 << STROBE; 	//set data direction for control pins (MSGEQ7)
	DDRB |= 1 << RESET;

	PORTB &= ~(1 << RESET); //reset pin low
	PORTB |= 1 << STROBE;   //strobe pin high

	PORTB &= ~(1 << S0); 	//clear select pins
	PORTB &= ~(1 << S1);
	PORTB &= ~(1 << S2);

	while (1)
	{
		PORTB |= 1 << RESET;   //reset pin high
		PORTB = ~(1 << RESET); //reset pin low
		_delay_us(74);

		for (int i = 0; i < 7; i++)
		{
			PORTB |= 1 << ENABLE;				//disable output of 74HC4051
			PORTB = (PORTB & 0b11000111);		//clear select pins (bits 3 to 5)
			PORTB += ((i & 0b001) << S0);		//set control pins for 74HC4051
			PORTB += ((i & 0b010) << (S1 - 1));
			PORTB += ((i & 0b100) << (S2 - 2));
			PORTB &= ~(1 << STROBE);			//strobe low --> get output from MSGEQ7
			_delay_us(36);					    //output settling time
			PORTB &= ~(1 << ENABLE);			//enable output of 74HC4051
			PORTB |= 1 << STROBE;			    //strobe high
			_delay_us(36);
		}
	}
}
