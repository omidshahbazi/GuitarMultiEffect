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
		enum class Types
		{
			Normal = 0,
			Reverse,
			PingPong
		};

	public:
		Data(void)
			: Type(Types::Normal)
		{
		}

	public:
		Types Type;
	};

public:
	DelEffect(void)
		: m_Filter(nullptr)
	{
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		// m_Filter->ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		switch (Data.Type)
		{
		case Data::Types::Normal:
			break;

		case Data::Types::Reverse:
			break;

		case Data::Types::PingPong:
			break;

		default:
			ASSERT(false, "Unhandled Type");
		}
	}

	cstr GetName(void) const override
	{
		return "DEL";
	}

private:
	Filter<SampleType, SAMPLE_RATE> *m_Filter;
};

#endif