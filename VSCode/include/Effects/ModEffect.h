#pragma once
#ifndef MOD_EFFECT_H
#define MOD_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Flanger.h"

class ModEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		static constexpr uint8 MAX_DEPTH = Flanger<SampleType, SAMPLE_RATE>::MAX_DEPTH;

	public:
		Data(void)
			: Depth(50),
			  Rate(2),
			  Feedback(0.5),
			  Wet(0.5)
		{
		}

	public:
		//[0, MAX_DEPTH]
		float Depth;
		//[0.01, 4]
		float Rate;
		//[0, 1]
		float Feedback;
		//[0, 1]
		float Wet;
	};

public:
	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_DSP.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_DSP.SetDepth(Data.Depth);
		m_DSP.SetRate(Data.Rate);
		m_DSP.SetFeedback(Data.Feedback);
		m_DSP.SetWetRate(Data.Wet);
	}

	cstr GetName(void) const override
	{
		return "MOD";
	}

private:
	Flanger<SampleType, SAMPLE_RATE> m_DSP;
};

#endif