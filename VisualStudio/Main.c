#include "Utils.h"

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

int main()
{
	SetPortDMode(true);
	SetPortBPinMode(1, true);

	BindMUXToADCPin(0);

	SetADCDivisionFactor(ADC_DIVISION_FACTOR_128);

	EnableADC();
	EnableADCFreeRun();
	StartADC();

	//InitializePWM(PWM_PIN_8BIT_1, PWM_MODE_FAST);

	 // Set PORTA as an output
	DDRB |= (1 << PB1);  // Assuming you are using PA0

	// Configure Timer0 for Fast PWM mode
	TCCR0 = (1 << WGM10) | (1 << WGM20) | (1 << COM1B0) | (1 << CS01) | (1 << CS00);

	// Set initial PWM value

	uint8 brightness = 0;
	while (1)
	{
		uint32 currADC = ReadADCValue();

		//SetPWM1(PWM_PIN_8BIT_1, currADC / 1024.0F, PWM_PRESCALER_8);

		while (brightness++ <= 100)
		{
			OCR1A = currADC / 1023.0F * 3;  // Update PWM value
			//SetPWM(PWM_PIN_8BIT_1, brightness / 100.0F);
			//Sleep(10); // Delay for smoother transition
		}

		//Sleep(10);
	}

	return 0;
}