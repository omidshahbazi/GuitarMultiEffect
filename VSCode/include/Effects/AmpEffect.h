#pragma once
#ifndef AMP_EFFECT_H
#define AMP_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/Filters/TripleToneControlFilter.h"
#include "../framework/DSP/Filters/SingleToneControlFilter.h"

class AmpEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		Data(void)
			: Gain(0),
			  LowTone(0),
			  MidTone(0),
			  HighTone(0),

			  PresenceRatio(0.5),
			  Master(1)
		{
		}

	public:
		//[-20dB, 40dB]
		float Gain;
		//[-20dB, 20dB]
		float LowTone;
		//[-20dB, 20dB]
		float MidTone;
		//[-20dB, 20dB]
		float HighTone;

		//[0, 1]
		float PresenceRatio;
		//[0, 1]
		float Master;
	};

public:
	AmpEffect(void)
		: m_GainMultiplier(0),
		  m_Master(0)
	{
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] = Math::Clamp(Buffer[i] * m_GainMultiplier, -1, 1);

		m_ToneControlFilter.ProcessBuffer(Buffer, Count);

		m_PresenceToneFilter.ProcessBuffer(Buffer, Count);

		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] *= m_Master;
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_GainMultiplier = Math::dbToMultiplier(Data.Gain);

		m_ToneControlFilter.SetLowTone(Data.LowTone);
		m_ToneControlFilter.SetMidTone(Data.MidTone);
		m_ToneControlFilter.SetHighTone(Data.HighTone);

		m_PresenceToneFilter.SetTone(Data.PresenceRatio);

		m_Master = Data.Master;
	}

	cstr GetName(void) const override
	{
		return "AMP";
	}

private:
	TripleToneControlFilter<SampleType, SAMPLE_RATE> m_ToneControlFilter;
	SingleToneControlFilter<SampleType, SAMPLE_RATE> m_PresenceToneFilter;
	float m_GainMultiplier;
	float m_Master;
};

#endif