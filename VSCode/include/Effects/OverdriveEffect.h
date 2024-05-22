#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Distortion.h"

class OverdriveEffect : public Effect
{
public:
	struct Data
	{
	public:
		float Rate;
		float Gain;
	};

public:
	OverdriveEffect(void)
		: m_Distortion(SAMPLE_RATE)
	{
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_Distortion.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		m_Distortion.SetRate(Data.Rate);
		m_Distortion.SetGain(Data.Gain);
	}

private:
	Distortion<SampleType> m_Distortion;
};

#endif
#pragma GCC pop_options