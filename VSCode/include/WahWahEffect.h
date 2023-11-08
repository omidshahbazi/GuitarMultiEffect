#pragma once
#ifndef WAH_WAH_EFFECT_H
#define WAH_WAH_EFFECT_H

#include "IEffect.h"
#include "framework/include/Wave/LowPassFilter.h"
#include "framework/include/Wave/Tables.h"

class WahWahEffect : public IEffect
{
public:
	WahWahEffect(void)
		: m_Position(0)
	{
		m_Step = (int32)(m_Frequency * TABLE_SIZE / 44100); // Assuming 44100 Hz sample rate
	}

	void Process(float *Buffer, uint16 Count) override ? ? ? ?
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float modulator = SINE_TABLE[m_Position];

			Buffer[i] = fixed_mul(Buffer[i], modulator);

			m_Position += m_Step;
			if (m_Position >= TABLE_SIZE)
				m_Position -= TABLE_SIZE;

			Buffer[i] = m_LowPassFilter.Process(Buffer[i]);
		}
	}

private:
	int16 fixed_mul(int16 a, int16 b)
	{
		return (a * b) >> 15;
	}

private:
	LowPassFilter m_LowPassFilter;
	int m_Position;
	int m_Frequency;
	int m_Step;
};

#endif