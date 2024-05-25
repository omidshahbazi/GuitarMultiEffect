#pragma once
#ifndef REVERB_EFFECT_H
#define REVERB_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Reverb.h"

class ReverbEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		float DelayTime;
		float Feedback;
		float WetRate;
	};

public:
	ReverbEffect(void)
		: m_DSP(SAMPLE_RATE, MAX_DELAY_TIME)
	{
	}

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

private:
	Reverb<SampleType> m_DSP;

public:
	static constexpr uint8 MAX_DELAY_TIME = 1;
};

#endif