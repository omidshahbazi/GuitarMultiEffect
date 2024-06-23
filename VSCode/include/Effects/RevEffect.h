#ifdef ADD_REV_EFFECT

#pragma once
#ifndef REV_EFFECT_H
#define REV_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Reverb.h"

class RevEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		static constexpr uint8 MAX_DELAY_TIME = 1;

	public:
		Data(void)
			: DelayTime(0.3),
			  Feedback(0.6),
			  WetRate(0.5)
		{
		}

	public:
		//[0, MAX_DELAY_TIME]
		float DelayTime;
		//[0, 1]
		float Feedback;
		//[0, 1]
		float WetRate;
	};

public:
	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_ReverbDSP.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_ReverbDSP.SetDelayTime(Data.DelayTime);
		m_ReverbDSP.SetFeedback(Data.Feedback);
		m_ReverbDSP.SetWetRate(Data.WetRate);

		if (Data.Enabled)
			m_ReverbDSP.Reset();
	}

	cstr GetName(void) const override
	{
		return "REV";
	}

private:
	Reverb<SampleType, SAMPLE_RATE, Data::MAX_DELAY_TIME> m_ReverbDSP;
};

#endif

#endif