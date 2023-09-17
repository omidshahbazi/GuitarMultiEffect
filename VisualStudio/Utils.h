#pragma once
#include <avr/io.h>
#include <util/delay.h>

#define uint8 unsigned char
#define uint32 unsigned int
#define bool char
#define true 1
#define false 0

#define EnableBit(Mask, BitIndex) (Mask |= (1 << BitIndex))
#define DisableBit(Mask, BitIndex) (Mask &= ~(1 << BitIndex))
#define ToggleBit(Mask, BitIndex) (Mask ^= (1 << BitIndex))
#define IsBitEnabled(Mask, BitIndex) ((Mask & (1 << BitIndex)) != 0)

#define SetPortBMode(Writable) (DDRB = (Writable ? 0xFF : 0))
#define SetPortCMode(Writable) (DDRC = (Writable ? 0xFF : 0))
#define SetPortDMode(Writable) (DDRD = (Writable ? 0xFF : 0))

#define SetPortBPinMode(PinIndex, Writable) (DDRB = (Writable ? DDRB | (1 << PinIndex) : DDRB & ~(1 << PinIndex))
#define SetPortCPinMode(PinIndex, Writable) (DDRC = (Writable ? DDRC | (1 << PinIndex) : DDRC & ~(1 << PinIndex))
#define SetPortDPinMode(PinIndex, Writable) (DDRD = (Writable ? DDRD | (1 << PinIndex) : DDRD & ~(1 << PinIndex))

#define ReadFromPortB() (PINB & 0b11111111)
#define ReadFromPortC() (PINC & 0b11111111)
#define ReadFromPortD() (PIND & 0b11111111)

#define ReadFromPortBPin(PinIndex) (PINB & (1 << PinIndex))
#define ReadFromPortCPin(PinIndex) (PINC & (1 << PinIndex))
#define ReadFromPortDPin(PinIndex) (PIND & (1 << PinIndex))

#define SetPortB() (PORTB = 0xFF)
#define SetPortC() (PORTC = 0xFF)
#define SetPortD() (PORTD = 0xFF)

#define SetPortBPin(PinIndex) EnableBit(PORTB, PinIndex)
#define SetPortCPin(PinIndex) EnableBit(PORTC, PinIndex)
#define SetPortDPin(PinIndex) EnableBit(PORTD, PinIndex)

#define ResetPortB() (PORTB = 0x00)
#define ResetPortC() (PORTC = 0x00)
#define ResetPortD() (PORTD = 0x00)

#define ResetPortBPin(PinIndex) DisableBit(PORTB, PinIndex)
#define ResetPortCPin(PinIndex) DisableBit(PORTC, PinIndex)
#define ResetPortDPin(PinIndex) DisableBit(PORTD, PinIndex)

void BindMUXToADCPin(uint32 PinIndex)
{
	switch (PinIndex)
	{
	case 0:
	{
		DisableBit(ADMUX, MUX0);
		DisableBit(ADMUX, MUX1);
		DisableBit(ADMUX, MUX2);
		DisableBit(ADMUX, MUX3);
	} break;

	case 1:
	{
		EnableBit(ADMUX, MUX0);
		DisableBit(ADMUX, MUX1);
		DisableBit(ADMUX, MUX2);
		DisableBit(ADMUX, MUX3);
	} break;

	case 2:
	{
		DisableBit(ADMUX, MUX0);
		EnableBit(ADMUX, MUX1);
		DisableBit(ADMUX, MUX2);
		DisableBit(ADMUX, MUX3);
	} break;

	case 3:
	{
		EnableBit(ADMUX, MUX0);
		EnableBit(ADMUX, MUX1);
		DisableBit(ADMUX, MUX2);
		DisableBit(ADMUX, MUX3);
	} break;

	case 4:
	{
		EnableBit(ADMUX, MUX0);
		DisableBit(ADMUX, MUX1);
		EnableBit(ADMUX, MUX2);
		DisableBit(ADMUX, MUX3);
	} break;

	case 5:
	{
		EnableBit(ADMUX, MUX0);
		EnableBit(ADMUX, MUX1);
		EnableBit(ADMUX, MUX2);
		EnableBit(ADMUX, MUX3);
	} break;
	}
}

void EnableUsingAVCC(void)
{
	DisableBit(ADMUX, REFS0);
	DisableBit(ADMUX, REFS1);
}

void EnableUsingVREF(void)
{
	EnableBit(ADMUX, REFS0);
	DisableBit(ADMUX, REFS1);
}

void EnableUsingInternal2_56v(void)
{
	EnableBit(ADMUX, REFS0);
	EnableBit(ADMUX, REFS1);
}

void EnableUsingInternal1_1v(void)
{
	DisableBit(ADMUX, REFS0);
	EnableBit(ADMUX, REFS1);
}

void EnableADC(void)
{
	EnableBit(ADCSRA, ADEN);
}

void DisableADC(void)
{
	DisableBit(ADCSRA, ADEN);
}

void StartADC(void)
{
	EnableBit(ADCSRA, ADSC);
}

void StopADC(void)
{
	DisableBit(ADCSRA, ADSC);
}

void EnableADCFreeRun(void)
{
	EnableBit(ADCSRA, ADFR);
}

void DisableADCFreeRun(void)
{
	DisableBit(ADCSRA, ADFR);
}

//2, 4, 8, 16, 32, 64, 128, the larger value, the slower ADC clock
void SetADCDivisionFactor(uint8 Value)
{
	switch (Value)
	{
	case 2:
	{
		EnableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case 4:
	{
		DisableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case 8:
	{
		EnableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case 16:
	{
		DisableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case 32:
	{
		EnableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case 64:
	{
		DisableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case 128:
	{
		EnableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;
	}
}

uint32 ReadADCValue(void)
{
	while (!IsBitEnabled(ADCSRA, ADIF));

	return ADC;
}

uint32 StartAndReadADCValue(void)
{
	StartADC();

	return ReadADCValue();
}

//4, 16, 64, 128
void EnableMasterSPI(uint8 Clock)
{
	EnableBit(SPCR, SPE);
	EnableBit(SPCR, MSTR);

	switch (Clock)
	{
	case 2:
	{
		DisableBit(SPCR, SPR0);
		DisableBit(SPCR, SPR1);
	} break;

	case 16:
	{
		EnableBit(SPCR, SPR0);
		DisableBit(SPCR, SPR1);
	} break;

	case 64:
	{
		DisableBit(SPCR, SPR0);
		EnableBit(SPCR, SPR1);
	} break;

	case 128:
	{
		EnableBit(SPCR, SPR0);
		EnableBit(SPCR, SPR1);
	} break;
	}
}

void DisableSPI()
{
	DisableBit(SPCR, SPE);
	DisableBit(SPCR, MSTR);
	DisableBit(SPCR, SPR0);
	DisableBit(SPCR, SPR1);
}

void TransmitSPI(uint8 Value)
{
	SPDR = Value;

	while (!IsBitEnabled(SPSR, SPIF));
}

void Sleep(uint32 Milliseconds)
{
	_delay_ms(Milliseconds);
}