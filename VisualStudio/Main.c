#include "Utils.h"
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

//void run()
//{
//	DDRD = 0xff;	// Set Port D for LED output
//	PORTD = 0x00;	// Clear Portd pins
//
//	//ADMUX = (1 << REFS1) | (1 << REFS0) | (0 << MUX0) | (0 << MUX1) | (0 << MUX2) | (0 << MUX3);
//
//	ADMUX = 0;
//	ADCSRA = 0;
//
//	//ADMUX |= (1 << MUX0);
//	//ADMUX |= (1 << MUX2);
//
//	ADCSRA |= (1 << ADPS0);
//	ADCSRA |= (1 << ADPS1);
//	ADCSRA |= (1 << ADPS2);
//
//	ADCSRA |= (1 << ADEN);
//	ADCSRA |= (1 << ADFR);
//	ADCSRA |= (1 << ADSC);
//
//
//	double firstADC = getADC();
//
//	while (1)	//Forever since it is in  single conversion mode
//	{
//		double currADC = getADC();	// Variable to hold ADC  result
//
//		if (currADC > firstADC)
//		{
//
//			TURN_ON_PORT(PORTD, 0);
//		}
//		else
//		{
//			TURN_OFF_PORT(PORTD, 0);
//		}
//	}
//}

void run1()
{
	SetPortDMode(true);

	BindMUXToADCPin(0);

	SetADCDivisionFactor(128);

	EnableADC();
	EnableADCFreeRun();
	StartADC();

	uint firstADC = ReadADCValue();

	while (1)
	{
		uint currADC = ReadADCValue();

		if (currADC > firstADC)
		{
			TurnOnPin(PORTD, 0);
		}
		else
		{
			TurnOffPin(PORTD, 0);
		}
	}
}

int main()
{
	run1();


	//DDRC = 0b00000000;
	//DDRB = 0b11111111;

	//while (1)
	//{
	//   int input = PINC & 0b00000001;  
	//   PORTB = (input != 0);
	//   PORTB = input;
	//}

	//DDRC = 0xFF;
	//DDRB = 0xFF;
	//int index = 0; 
	//while (1)
	//{
	//   index = (++index % 8);
	//   PORTB = 1 << index; 
	//_delay_ms(1500);
	//}

	return 0;
}