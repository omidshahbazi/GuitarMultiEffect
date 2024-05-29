#pragma once
#ifndef REVERB_EFFECT_H
#define REVERB_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Reverb.h"

class ReverbEffect : public Effect
{
public:
	static constexpr uint8 MAX_DELAY_TIME = 1;

public:
	struct Data : public Effect::Data
	{
	public:
		float DelayTime;
		float Feedback;
		float WetRate;
	};

public:
	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_DSP.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_DSP.SetDelayTime(Data.DelayTime);
		m_DSP.SetFeedback(Data.Feedback);
		m_DSP.SetWetRate(Data.WetRate);
	}

	cstr GetName(void) const override
	{
		return "REV";
	}

private:
	Reverb<SampleType, SAMPLE_RATE, MAX_DELAY_TIME> m_DSP;
};

#endif