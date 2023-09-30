#pragma once
#include <avr/io.h>
#include <util/delay.h>

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define bool uint8_t
#define true 1
#define false 0

#define ADC_DIVISION_FACTOR_2 2
#define ADC_DIVISION_FACTOR_4 4
#define ADC_DIVISION_FACTOR_8 8
#define ADC_DIVISION_FACTOR_16 16
#define ADC_DIVISION_FACTOR_32 32
#define ADC_DIVISION_FACTOR_64 64
#define ADC_DIVISION_FACTOR_128 128

#define PWM_PIN_8BIT_1 1
#define PWM_PIN_8BIT_2 2
#define PWM_PIN_16BIT 3

#define PWM_MODE_FAST 0
#define PWM_MODE_PHASE_CORRECT 1
#define PWM_MODE_FREQ_AND_PHASE_CORRECT 2

#define PWM_PRESCALER_OFF 1
#define PWM_PRESCALER_8 8
#define PWM_PRESCALER_64 64
#define PWM_PRESCALER_256 256
#define PWM_PRESCALER_1024 1024

#define EnableBit(Mask, BitIndex) (Mask |= (1 << BitIndex))
#define DisableBit(Mask, BitIndex) (Mask &= ~(1 << BitIndex))
#define ToggleBit(Mask, BitIndex) (Mask ^= (1 << BitIndex))
#define IsBitEnabled(Mask, BitIndex) (Mask & (1 << BitIndex))

#define SetPortBMode(Writable) (DDRB = (Writable ? 0xFF : 0))
#define SetPortCMode(Writable) (DDRC = (Writable ? 0xFF : 0))
#define SetPortDMode(Writable) (DDRD = (Writable ? 0xFF : 0))

#define SetPortBPinMode(PinIndex, Writable) (DDRB = (Writable ? DDRB | (1 << PinIndex) : DDRB & ~(1 << PinIndex)))
#define SetPortCPinMode(PinIndex, Writable) (DDRC = (Writable ? DDRC | (1 << PinIndex) : DDRC & ~(1 << PinIndex)))
#define SetPortDPinMode(PinIndex, Writable) (DDRD = (Writable ? DDRD | (1 << PinIndex) : DDRD & ~(1 << PinIndex)))

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

//The larger value, the slower ADC clock
void SetADCDivisionFactor(uint8 Value)
{
	switch (Value)
	{
	case ADC_DIVISION_FACTOR_2:
	{
		EnableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_4:
	{
		DisableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_8:
	{
		EnableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		DisableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_16:
	{
		DisableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_32:
	{
		EnableBit(ADCSRA, ADPS0);
		DisableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_64:
	{
		DisableBit(ADCSRA, ADPS0);
		EnableBit(ADCSRA, ADPS1);
		EnableBit(ADCSRA, ADPS2);
	} break;

	case ADC_DIVISION_FACTOR_128:
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

	EnableBit(SPCR, SPE);
	EnableBit(SPCR, MSTR);
}

void DisableSPI()
{
	DisableBit(SPCR, SPE);
	DisableBit(SPCR, MSTR);
	DisableBit(SPCR, SPR0);
	DisableBit(SPCR, SPR1);
}

void TransmitSPI(uint16 Value)
{
	SPDR = (uint8)(Value >> 8);

	while (!IsBitEnabled(SPSR, SPIF));

	SPDR = (uint8)Value;

	while (!IsBitEnabled(SPSR, SPIF));
}

void InitializePWM(uint8 Pin, uint8 Mode)
{
	TCCR1A &= ~((1 << WGM10) | (1 << WGM11) | (1 << COM1A1));
	TCCR1B &= ~((1 << WGM12) | (1 << WGM13));

	switch (Mode)
	{
	case PWM_MODE_FAST:
	{
		TCCR1A |= (1 << COM1A1) | (1 << WGM10);
		TCCR1B |= (1 << WGM12);
	} break;

	case PWM_MODE_PHASE_CORRECT:
	{
		TCCR1A |= (1 << COM1A1);
		TCCR1B |= (1 << WGM13);
	} break;

	case PWM_MODE_FREQ_AND_PHASE_CORRECT:
	{
		TCCR1A |= (1 << COM1A1) | (1 << WGM10);
		TCCR1B |= (1 << WGM13);
	} break;
	}

	TCCR1B |= (1 << CS11) | (1 << CS10);
}

//0.0-1.0
void SetPWM(uint8 Pin, float DutyCycle)
{
	uint16 compareValue = 255 * DutyCycle;

	switch (Pin)
	{
	case PWM_PIN_8BIT_1:
		OCR1A = compareValue;
		break;

	case PWM_PIN_8BIT_2:
		OCR1B = compareValue;
		break;

	case PWM_PIN_16BIT:
		ICR1 = compareValue;
		break;
	}

	TCCR1B |= (1 << CS11) | (1 << CS10);
}

void SetPWM1(uint8 Pin, float Frequency, uint8 Prescaler)
{
	uint16 top = F_CPU / (Frequency * 2 * Prescaler) - 1;

	switch (Pin)
	{
	case PWM_PIN_8BIT_1:
		OCR1A = top;
		break;

	case PWM_PIN_8BIT_2:
		OCR1B = top;
		break;

	case PWM_PIN_16BIT:
		ICR1 = top;
		break;
	}
}

void Sleep(uint32 Milliseconds)
{
	_delay_ms(Milliseconds);
}