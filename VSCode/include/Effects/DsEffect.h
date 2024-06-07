#pragma once
#ifndef DS_EFFECT_H
#define DS_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Overdrive.h"
#include "../framework/DSP/DSPs/Distortion.h"

class DsEffect : public Effect
{
public:
	struct Data : public Effect::Data
	{
	public:
		enum class Types
		{
			Overdrive = 0,
			Distortion
		};

	public:
		Data(void)
			: Type(Types::Distortion),

			  OverdriveDrive(1),
			  OverdriveGain(1),

			  DistortionLevel(1),
			  DistortionGain(1)
		{
		}

	public:
		Types Type;

		//[0, 1]
		float OverdriveDrive;
		//[0, 1]
		float OverdriveGain;

		//[0, 1]
		float DistortionLevel;
		//[-10dB, 10dB]
		float DistortionGain;
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

		default:
			ASSERT(false, "Unhandled Type");
		}

		m_OverdriveDSP.SetDrive(Data.OverdriveDrive);
		m_OverdriveDSP.SetGain(Data.OverdriveGain);

		m_DistortionDSP.SetLevel(Data.DistortionLevel);
		m_DistortionDSP.SetGain(Data.DistortionGain);
	}

	cstr GetName(void) const override
	{
		return "DS";
	}

private:
	Overdrive<SampleType, SAMPLE_RATE> m_OverdriveDSP;
	Distortion<SampleType, SAMPLE_RATE> m_DistortionDSP;
	IDSP<SampleType, SAMPLE_RATE> *m_DSP;
};

#endif