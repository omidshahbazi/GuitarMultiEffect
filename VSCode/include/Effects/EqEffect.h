#pragma once
#ifndef EQ_EFFECT_H
#define EQ_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/Filters/TripleToneControlFilter.h"

class EqEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		Data(void)
			: LowTone(0),
			  MidTone(0),
			  HighTone(0)
		{
		}

	public:
		//[-20, 20]
		float LowTone;
		//[-20, 20]
		float MidTone;
		//[-20, 20]
		float HighTone;
	};

public:
	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_DSP.ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_DSP.SetLowTone(Data.LowTone);
		m_DSP.SetMidTone(Data.MidTone);
		m_DSP.SetHighTone(Data.HighTone);
	}

	cstr GetName(void) const override
	{
		return "EQ";
	}

private:
	TripleToneControlFilter<SampleType, SAMPLE_RATE> m_DSP;
};

#endif