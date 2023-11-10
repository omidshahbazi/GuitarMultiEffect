#pragma once
#ifndef WAH_WAH_EFFECT_H
#define WAH_WAH_EFFECT_H

#include "Effect.h"
#include "framework/include/Wave/LowPassFilter.h"
#include "framework/include/Wave/Tables.h"

class WahWahEffect : public Effect
{
public:
	WahWahEffect(uint32 SampleRate)
		: m_Frequency(0),
		  m_SampleRate(SampleRate),
		  m_Step(0),
		  m_Position(0)
	{
		SetFrequency(5000);
	}

	void SetFrequency(float Value)
	{
		m_Frequency = Value;

		m_LowPassFilter.SetCutoffFrequencye(m_Frequency);

		m_Step = (int32)(m_Frequency * TABLE_SIZE / m_SampleRate);
	}

	float GetDelayTime(void) const
	{
		return m_Frequency;
	}

protected:
	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float modulator = SINE_TABLE[m_Position];

			// Log::WriteInfo("Output %f,%f,%f", Buffer[i], modulator, Buffer[i] * modulator);

			Buffer[i] = m_LowPassFilter.Process(Buffer[i] * modulator);

			m_Position += m_Step;
			if (m_Position >= TABLE_SIZE)
				m_Position -= TABLE_SIZE;
		}
	}

private:
	LowPassFilter m_LowPassFilter;
	float m_Frequency;
	uint32 m_SampleRate;
	int32 m_Step;
	int32 m_Position;
};

#endif