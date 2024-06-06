#pragma once
#ifndef FX_EFFECT_H
#define FX_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/AutoWah.h"

class FXEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		enum class Types
		{
			AutoWah = 0,
			CryBaby
		};

	public:
		Data(void)
			: Type(Types::AutoWah),
			  WahRatio(0)
		{
		}

	public:
		Types Type;

		//[0, 1]
		float WahRatio;
	};

public:
	FXEffect(void)
		: m_DSP(nullptr)
	{
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		m_DSP->ProcessBuffer(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		m_Wah.SetRatio(Data.WahRatio);

		switch (Data.Type)
		{
		case Data::Types::AutoWah:
			m_DSP = reinterpret_cast<Wah<SampleType, SAMPLE_RATE> *>(&m_AutoWah);
			break;

		case Data::Types::CryBaby:
			m_Wah.SetType(Wah<SampleType, SAMPLE_RATE>::Types::CryBaby);
			m_DSP = &m_Wah;
			break;

		default:
			ASSERT(false, "Unhandled Type");
		}
	}

	cstr GetName(void) const override
	{
		return "FX";
	}

private:
	Wah<SampleType, SAMPLE_RATE> m_Wah;
	AutoWah<SampleType, SAMPLE_RATE> m_AutoWah;
	IDSP<SampleType, SAMPLE_RATE> *m_DSP;
};

#endif