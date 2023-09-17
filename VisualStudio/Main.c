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
	SetPortBMode(true);

	BindMUXToADCPin(0);

	SetADCDivisionFactor(128);

	EnableADC();
	EnableADCFreeRun();
	StartADC();

	EnableMasterSPI(16);

	while (1)
	{
		uint32 currADC = ReadADCValue();

		TransmitSPI(currADC >> 2);
	}

	return 0;
}