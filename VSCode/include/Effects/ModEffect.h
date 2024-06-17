#ifdef ADD_MOD_EFFECT

#pragma once
#ifndef MOD_EFFECT_H
#define MOD_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Chorus.h"
#include "../framework/DSP/DSPs/Flanger.h"
#include "../framework/DSP/DSPs/Phaser.h"
#include "../framework/DSP/DSPs/Tremolo.h"

class ModEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		enum class Types
		{
			Chorus = 0,
			Flanger,
			Phaser,
			Tremolo
		};

	public:
		Data(void)
			: Type(Types::Flanger),

			  ChorusDepth(50),
			  ChorusRate(2),
			  ChorusWetRate(0.5),

			  FlangerDepth(50),
			  FlangerRate(2),
			  FlangerFeedback(0.5),
			  FlangerWetRate(0.5),

			  PhaserDepth(50),
			  PhaserRate(2),
			  PhaserWetRate(0.5),

			  TremoloDepth(0.5),
			  TremoloRate(12),
			  TremoloWetRate(0.5)
		{
		}

	public:
		Types Type;

		//[0, 100]
		float ChorusDepth;
		//[0.01, 4]
		float ChorusRate;
		//[0, 1]
		float ChorusWetRate;

		//[0, 100]
		float FlangerDepth;
		//[0.01, 4]
		float FlangerRate;
		//[0, 1]
		float FlangerFeedback;
		//[0, 1]
		float FlangerWetRate;

		//[0, 100]
		float PhaserDepth;
		//[0.1, 3.7]
		float PhaserRate;
		//[0, 1]
		float PhaserWetRate;

		//[0, 1]
		float TremoloDepth;
		//[1, 25]
		float TremoloRate;
		//[0, 1]
		float TremoloWetRate;
	};

public:
	ModEffect(void)
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

		switch (Data.Type)
		{
		case Data::Types::Chorus:
			m_DSP = &m_ChorusDSP;
			break;

		case Data::Types::Flanger:
			m_DSP = &m_FlangerDSP;
			break;

		case Data::Types::Phaser:
			m_DSP = &m_PhaserDSP;
			break;

		case Data::Types::Tremolo:
			m_DSP = &m_TremoloDSP;
			break;

		default:
			ASSERT(false, "Unhandled Type");
		}

		m_ChorusDSP.SetDepth(Data.ChorusDepth);
		m_ChorusDSP.SetRate(Data.ChorusRate);
		m_ChorusDSP.SetWetRate(Data.ChorusWetRate);

		m_FlangerDSP.SetDepth(Data.FlangerDepth);
		m_FlangerDSP.SetRate(Data.FlangerRate);
		m_FlangerDSP.SetFeedback(Data.FlangerFeedback);
		m_FlangerDSP.SetWetRate(Data.FlangerWetRate);

		m_PhaserDSP.SetDepth(Data.PhaserDepth);
		m_PhaserDSP.SetRate(Data.PhaserRate);
		m_PhaserDSP.SetWetRate(Data.PhaserWetRate);

		m_TremoloDSP.SetDepth(Data.TremoloDepth);
		m_TremoloDSP.SetRate(Data.TremoloRate);
		m_TremoloDSP.SetWetRate(Data.TremoloWetRate);
	}

	cstr GetName(void) const override
	{
		return "MOD";
	}

private:
	Chorus<SampleType, SAMPLE_RATE> m_ChorusDSP;
	Flanger<SampleType, SAMPLE_RATE> m_FlangerDSP;
	Phaser<SampleType, SAMPLE_RATE> m_PhaserDSP;
	Tremolo<SampleType, SAMPLE_RATE> m_TremoloDSP;
	IDSP<SampleType, SAMPLE_RATE> *m_DSP;
};

#endif

#endif