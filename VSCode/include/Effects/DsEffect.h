#ifdef ADD_DS_EFFECT

#pragma once
#ifndef DS_EFFECT_H
#define DS_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Overdrive.h"
#include "../framework/DSP/DSPs/Distortion.h"
#include "../framework/DSP/DSPs/Fuzz.h"

class DsEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		enum class Types
		{
			Overdrive = 0,
			Distortion,
			Fuzz
		};

	public:
		Data(void)
			: Type(Types::Distortion),

			  OverdriveDrive(1),
			  OverdriveGain(0),

			  DistortionLevel(0.5),
			  DistortionGain(0),

			  FuzzTone(0.5),
			  FuzzLevel(0),
			  FuzzWet(0.5)
		{
		}

	public:
		Types Type;

		//[0, 1]
		float OverdriveDrive;
		//[-6dB, 2dB]
		float OverdriveGain;

		//[0, 1]
		float DistortionLevel;
		//[-20dB, 10dB]
		float DistortionGain;

		//[0, 1]
		float FuzzTone;
		//[0, 1]
		float FuzzLevel;
		//[0, 1]
		float FuzzWet;
	};

public:
	DsEffect(void)
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
		case Data::Types::Overdrive:
			m_DSP = &m_OverdriveDSP;
			break;

		case Data::Types::Distortion:
			m_DSP = &m_DistortionDSP;
			break;

		case Data::Types::Fuzz:
			m_DSP = &m_FuzzDSP;
			break;

		default:
			ASSERT(false, "Unhandled Type");
		}

		m_OverdriveDSP.SetDrive(Data.OverdriveDrive);
		m_OverdriveDSP.SetGain(Data.OverdriveGain);

		m_DistortionDSP.SetLevel(Data.DistortionLevel);
		m_DistortionDSP.SetGain(Data.DistortionGain);

		m_FuzzDSP.SetTone(Data.FuzzTone);
		m_FuzzDSP.SetLevel(Data.FuzzLevel);
		m_FuzzDSP.SetWetRate(Data.FuzzWet);
	}

	cstr GetName(void) const override
	{
		return "DS";
	}

private:
	Overdrive<SampleType, SAMPLE_RATE> m_OverdriveDSP;
	Distortion<SampleType, SAMPLE_RATE> m_DistortionDSP;
	Fuzz<SampleType, SAMPLE_RATE> m_FuzzDSP;
	IDSP<SampleType, SAMPLE_RATE> *m_DSP;
};

#endif

#endif