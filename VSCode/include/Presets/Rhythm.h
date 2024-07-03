#pragma once
#ifndef RHYTHM_H
#define RHYTHM_H

#include "../Common.h"
#include "../framework/DSP/Drums/DrumsMachine.h"

class Rhythm
{
private:
	typedef DrumsMachine<SampleType, SAMPLE_RATE> Drums;
	typedef Drums::Parts Parts;

public:
	enum class Presets
	{
		HalfNote1 = 0,
		HalfNote2,
		HalfNote3,
		HalfNote4,
		QuarterNote1,
		QuarterNote2,
		QuarterNote3,
		QuarterNote4
	};

public:
	struct Data
	{
	public:
		Data(void)
			: IsEnabledInteger(0),
			  Preset(Presets::HalfNote1),
			  BeatsPerMinute(60),
			  Volume(0.1)
		{
		}

		union
		{
			bool IsEnabled;
			int32 IsEnabledInteger;
		};

		Presets Preset;
		//[20, 400]
		float BeatsPerMinute;
		//[0, 1]
		float Volume;
	};

public:
	Rhythm(IHAL *HAL)
		: m_Data{},
		  m_DrumsMachine(HAL)
	{
		m_DrumsMachine.SetEnabledParts(Parts::Kick | Parts::Snare | Parts::CymbalHiHat);
	}

	void Update(void)
	{
		if (!m_Data.IsEnabled)
			return;

		m_DrumsMachine.Update();
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		if (!m_Data.IsEnabled)
			return;

		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] = Math::Lerp(Buffer[i], m_DrumsMachine.Process(), m_Data.Volume * 0.5);
	}

	void SetData(const Data &Data)
	{
		m_Data = Data;

		static Parts Pattern1[8];
		Pattern1[0] = Parts::Kick | Parts::CymbalHiHat;
		Pattern1[1] = Parts::None;
		Pattern1[2] = Parts::Kick | Parts::Snare;
		Pattern1[3] = Parts::None;
		Pattern1[4] = Parts::Kick | Parts::CymbalHiHat;
		Pattern1[5] = Parts::None;
		Pattern1[6] = Parts::Kick | Parts::Snare;
		Pattern1[7] = Parts::None;

		static Parts Pattern2[8];
		Pattern2[0] = Parts::CymbalHiHat | Parts::Kick;
		Pattern2[1] = Parts::CymbalHiHat;
		Pattern2[2] = Parts::CymbalHiHat | Parts::Snare;
		Pattern2[3] = Parts::CymbalHiHat;
		Pattern2[4] = Parts::CymbalHiHat | Parts::Kick;
		Pattern2[5] = Parts::CymbalHiHat;
		Pattern2[6] = Parts::CymbalHiHat | Parts::Snare;
		Pattern2[7] = Parts::CymbalHiHat;

		static Parts Pattern3[8];
		Pattern3[0] = Parts::CymbalHiHat | Parts::Kick;
		Pattern3[1] = Parts::CymbalHiHat;
		Pattern3[2] = Parts::CymbalHiHat | Parts::Snare;
		Pattern3[3] = Parts::CymbalHiHat | Parts::Kick;
		Pattern3[4] = Parts::CymbalHiHat | Parts::Kick;
		Pattern3[5] = Parts::CymbalHiHat | Parts::Kick;
		Pattern3[6] = Parts::CymbalHiHat | Parts::Snare;
		Pattern3[7] = Parts::CymbalHiHat;

		static Parts Pattern4[8];
		Pattern4[0] = Parts::CymbalHiHat | Parts::Kick;
		Pattern4[1] = Parts::CymbalHiHat | Parts::Kick;
		Pattern4[2] = Parts::CymbalHiHat | Parts::Snare;
		Pattern4[3] = Parts::CymbalHiHat | Parts::Kick;
		Pattern4[4] = Parts::CymbalHiHat;
		Pattern4[5] = Parts::CymbalHiHat;
		Pattern4[6] = Parts::CymbalHiHat | Parts::Snare;
		Pattern4[7] = Parts::CymbalHiHat | Parts::Kick;

		switch (m_Data.Preset)
		{
		case Presets::HalfNote1:
			m_DrumsMachine.SetNotes(Pattern1, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Half);
			break;

		case Presets::HalfNote2:
			m_DrumsMachine.SetNotes(Pattern2, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Half);
			break;

		case Presets::HalfNote3:
			m_DrumsMachine.SetNotes(Pattern3, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Half);
			break;

		case Presets::HalfNote4:
			m_DrumsMachine.SetNotes(Pattern4, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Half);
			break;

		case Presets::QuarterNote1:
			m_DrumsMachine.SetNotes(Pattern1, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Quarter);
			break;

		case Presets::QuarterNote2:
			m_DrumsMachine.SetNotes(Pattern2, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Quarter);
			break;

		case Presets::QuarterNote3:
			m_DrumsMachine.SetNotes(Pattern3, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Quarter);
			break;

		case Presets::QuarterNote4:
			m_DrumsMachine.SetNotes(Pattern4, 8);
			m_DrumsMachine.SetNoteDuration(Drums::NoteDurations::Quarter);
			break;

		default:
			ASSERT(false, "Unhandled Type");
		}

		m_DrumsMachine.SetBeatsPerMinute(m_Data.BeatsPerMinute);
	}

	Data &GetData(void)
	{
		return m_Data;
	}

	void UpdateData(void)
	{
		SetData(GetData());
	}

private:
	Data m_Data;
	DrumsMachine<SampleType, SAMPLE_RATE> m_DrumsMachine;
};

#endif