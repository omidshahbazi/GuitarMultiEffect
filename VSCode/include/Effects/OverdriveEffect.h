#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Distortion.h"

class OverdriveEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		float Rate;
		float Gain;
	};

public:
	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_DSP.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_DSP.SetRate(Data.Rate);
		m_DSP.SetGain(Data.Gain);
	}

	cstr GetName(void) const override
	{
		return "OD";
	}

private:
	Distortion<SampleType, SAMPLE_RATE> m_DSP;
};

#endif