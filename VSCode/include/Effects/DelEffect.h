#ifdef ADD_DEL_EFFECT

#pragma once
#ifndef DEL_EFFECT_H
#define DEL_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/Filters/DelayFilter.h"

class DelEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		static constexpr uint8 MAX_DELAY_TIME = 4;

	public:
		enum class Types
		{
			Normal = 0,
			Reverse
		};

	public:
		Data(void)
			: Type(Types::Normal),
			  DelayTime(1),
			  Feedback(0.7)
		{
		}

	public:
		Types Type;

		//[0, MAX_DELAY_TIME]
		float DelayTime;
		//[0, 1]
		float Feedback;
	};

public:
	DelEffect(void)
	{
		m_Delay.SetOutputMixRate(1);
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Delay.Process(Buffer[i], true);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_Delay.SetTime(Data.DelayTime);
		m_Delay.SetFeedback(Data.Feedback);
		m_Delay.SetReverse(Data.Type == Data::Types::Reverse);

		if (Data.Enabled)
			m_Delay.Reset();
	}

	cstr GetName(void) const override
	{
		return "DEL";
	}

private:
	DelayFilter<SampleType, SAMPLE_RATE, Data::MAX_DELAY_TIME> m_Delay;
};

#endif

#endif