#ifdef ADD_EQ_EFFECT

#pragma once
#ifndef EQ_EFFECT_H
#define EQ_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/Filters/BandPassFilter.h"
#include "../framework/DSP/Filters/HighPassFilter.h"

class EqEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		enum class Types
		{
			Boosting = 0,
			BoostingAlternative,
			CryingChicken,
			CryingChickenVocal,
			Bathory,
			Behemoth,
			DarkThrone,
			Mayhem,
			DimmuBorgir,
			Burzum,
			Emperor,
			Custom
		};

	public:
		Data(void)
			: Gain100(0),
			  Gain200(0),
			  Gain400(0),
			  Gain800(0),
			  Gain1_6K(0),
			  Gain3_2K(0),
			  Gain6_4K(0),
			  Gain(0)
		{
		}

	public:
		Types Type;
		//[-15, 15]
		float Gain100;
		//[-15, 15]
		float Gain200;
		//[-15, 15]
		float Gain400;
		//[-15, 15]
		float Gain800;
		//[-15, 15]
		float Gain1_6K;
		//[-15, 15]
		float Gain3_2K;
		//[-15, 15]
		float Gain6_4K;
		//[-15, 15]
		float Gain;
	};

public:
	EqEffect(void)
	{
		const float RESONANCE = 3;

		m_Band100.SetFrequencies(100, 200);
		m_Band100.SetResonance(RESONANCE);

		m_Band200.SetFrequencies(200, 400);
		m_Band200.SetResonance(RESONANCE);

		m_Band400.SetFrequencies(400, 800);
		m_Band400.SetResonance(RESONANCE);

		m_Band800.SetFrequencies(800, 1.6 * KHz);
		m_Band800.SetResonance(RESONANCE);

		m_Band1_6K.SetFrequencies(1.6 * KHz, 3.2 * KHz);
		m_Band1_6K.SetResonance(RESONANCE);

		m_Band3_2K.SetFrequencies(3.2 * KHz, 6.4 * KHz);
		m_Band3_2K.SetResonance(RESONANCE);

		m_Band6_4K.SetCutoffFrequency(6.4 * KHz);
		m_Band6_4K.SetResonance(RESONANCE);
	}

	void Process(SampleType *Buffer, uint8 Count) override
	{
		for (uint8 i = 0; i < Count; ++i)
		{
			SampleType input = Buffer[i];
			SampleType output = 0;

			output += m_Band100.Process(input) * m_Multiplier100;
			output += m_Band200.Process(input) * m_Multiplier200;
			output += m_Band400.Process(input) * m_Multiplier400;
			output += m_Band800.Process(input) * m_Multiplier800;
			output += m_Band1_6K.Process(input) * m_Multiplier1_6K;
			output += m_Band3_2K.Process(input) * m_Multiplier3_2K;
			output += m_Band6_4K.Process(input) * m_Multiplier6_4K;
			output *= m_Multiplier;

			Buffer[i] = output;
		}
	}

	void SetData(const Data &Data)
	{
		Effect::SetData(Data);

		switch (Data.Type)
		{
		case Data::Types::Boosting:
		{
			m_Multiplier100 = Math::dbToMultiplier(-5.0);
			m_Multiplier200 = Math::dbToMultiplier(0.0);
			m_Multiplier400 = Math::dbToMultiplier(4.0);
			m_Multiplier800 = Math::dbToMultiplier(2.0);
			m_Multiplier1_6K = Math::dbToMultiplier(0.0);
			m_Multiplier3_2K = Math::dbToMultiplier(-2.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-8.0);
			m_Multiplier = Math::dbToMultiplier(7.0);
		}
		break;

		case Data::Types::BoostingAlternative:
		{
			m_Multiplier100 = Math::dbToMultiplier(0.0);
			m_Multiplier200 = Math::dbToMultiplier(7.0);
			m_Multiplier400 = Math::dbToMultiplier(10.0);
			m_Multiplier800 = Math::dbToMultiplier(8.0);
			m_Multiplier1_6K = Math::dbToMultiplier(7.0);
			m_Multiplier3_2K = Math::dbToMultiplier(6.0);
			m_Multiplier6_4K = Math::dbToMultiplier(1.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::CryingChicken:
		{
			m_Multiplier100 = Math::dbToMultiplier(-12.0);
			m_Multiplier200 = Math::dbToMultiplier(0.0);
			m_Multiplier400 = Math::dbToMultiplier(6.0);
			m_Multiplier800 = Math::dbToMultiplier(-2.0);
			m_Multiplier1_6K = Math::dbToMultiplier(-9.0);
			m_Multiplier3_2K = Math::dbToMultiplier(-15.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-15.0);
			m_Multiplier = Math::dbToMultiplier(6.0);
		}
		break;

		case Data::Types::CryingChickenVocal:
		{
			m_Multiplier100 = Math::dbToMultiplier(-15.0);
			m_Multiplier200 = Math::dbToMultiplier(-5.0);
			m_Multiplier400 = Math::dbToMultiplier(7.0);
			m_Multiplier800 = Math::dbToMultiplier(-5.0);
			m_Multiplier1_6K = Math::dbToMultiplier(-15.0);
			m_Multiplier3_2K = Math::dbToMultiplier(-15.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-15.0);
			m_Multiplier = Math::dbToMultiplier(3.0);
		}
		break;

		case Data::Types::Bathory:
		{
			m_Multiplier100 = Math::dbToMultiplier(-5.0);
			m_Multiplier200 = Math::dbToMultiplier(-14.0);
			m_Multiplier400 = Math::dbToMultiplier(-7.0);
			m_Multiplier800 = Math::dbToMultiplier(-5.0);
			m_Multiplier1_6K = Math::dbToMultiplier(10.0);
			m_Multiplier3_2K = Math::dbToMultiplier(5.0);
			m_Multiplier6_4K = Math::dbToMultiplier(0.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::Behemoth:
		{
			m_Multiplier100 = Math::dbToMultiplier(2.0);
			m_Multiplier200 = Math::dbToMultiplier(-3.0);
			m_Multiplier400 = Math::dbToMultiplier(-4.0);
			m_Multiplier800 = Math::dbToMultiplier(5.0);
			m_Multiplier1_6K = Math::dbToMultiplier(7.0);
			m_Multiplier3_2K = Math::dbToMultiplier(-2.0);
			m_Multiplier6_4K = Math::dbToMultiplier(7.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::DarkThrone:
		{
			m_Multiplier100 = Math::dbToMultiplier(-10.0);
			m_Multiplier200 = Math::dbToMultiplier(-5.0);
			m_Multiplier400 = Math::dbToMultiplier(-3.0);
			m_Multiplier800 = Math::dbToMultiplier(10.0);
			m_Multiplier1_6K = Math::dbToMultiplier(-4.0);
			m_Multiplier3_2K = Math::dbToMultiplier(6.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-5.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::Mayhem:
		{
			m_Multiplier100 = Math::dbToMultiplier(2.0);
			m_Multiplier200 = Math::dbToMultiplier(-5.0);
			m_Multiplier400 = Math::dbToMultiplier(-4.0);
			m_Multiplier800 = Math::dbToMultiplier(3.0);
			m_Multiplier1_6K = Math::dbToMultiplier(-10.0);
			m_Multiplier3_2K = Math::dbToMultiplier(10.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-5.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::DimmuBorgir:
		{
			m_Multiplier100 = Math::dbToMultiplier(-2.0);
			m_Multiplier200 = Math::dbToMultiplier(-5.0);
			m_Multiplier400 = Math::dbToMultiplier(-4.0);
			m_Multiplier800 = Math::dbToMultiplier(1.0);
			m_Multiplier1_6K = Math::dbToMultiplier(4.0);
			m_Multiplier3_2K = Math::dbToMultiplier(1.0);
			m_Multiplier6_4K = Math::dbToMultiplier(1.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::Burzum:
		{
			m_Multiplier100 = Math::dbToMultiplier(2.0);
			m_Multiplier200 = Math::dbToMultiplier(2.0);
			m_Multiplier400 = Math::dbToMultiplier(-7.0);
			m_Multiplier800 = Math::dbToMultiplier(7.0);
			m_Multiplier1_6K = Math::dbToMultiplier(10.0);
			m_Multiplier3_2K = Math::dbToMultiplier(1.0);
			m_Multiplier6_4K = Math::dbToMultiplier(11.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::Emperor:
		{
			m_Multiplier100 = Math::dbToMultiplier(2.0);
			m_Multiplier200 = Math::dbToMultiplier(5.0);
			m_Multiplier400 = Math::dbToMultiplier(-7.0);
			m_Multiplier800 = Math::dbToMultiplier(-6.0);
			m_Multiplier1_6K = Math::dbToMultiplier(4.0);
			m_Multiplier3_2K = Math::dbToMultiplier(6.0);
			m_Multiplier6_4K = Math::dbToMultiplier(-4.0);
			m_Multiplier = Math::dbToMultiplier(0.0);
		}
		break;

		case Data::Types::Custom:
		{
			m_Multiplier100 = Math::dbToMultiplier(Data.Gain100);
			m_Multiplier200 = Math::dbToMultiplier(Data.Gain200);
			m_Multiplier400 = Math::dbToMultiplier(Data.Gain400);
			m_Multiplier800 = Math::dbToMultiplier(Data.Gain800);
			m_Multiplier1_6K = Math::dbToMultiplier(Data.Gain1_6K);
			m_Multiplier3_2K = Math::dbToMultiplier(Data.Gain3_2K);
			m_Multiplier6_4K = Math::dbToMultiplier(Data.Gain6_4K);
			m_Multiplier = Math::dbToMultiplier(Data.Gain);
		}
		break;
		}
	}

	cstr GetName(void) const override
	{
		return "EQ";
	}

private:
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band100;
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band200;
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band400;
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band800;
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band1_6K;
	BandPassFilter<SampleType, SAMPLE_RATE> m_Band3_2K;
	HighPassFilter<SampleType, SAMPLE_RATE> m_Band6_4K;

	float m_Multiplier100;
	float m_Multiplier200;
	float m_Multiplier400;
	float m_Multiplier800;
	float m_Multiplier1_6K;
	float m_Multiplier3_2K;
	float m_Multiplier6_4K;
	float m_Multiplier;
};

#endif

#endif