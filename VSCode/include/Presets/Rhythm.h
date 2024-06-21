#pragma once
#ifndef RHYTHM_H
#define RHYTHM_H

#include "../Common.h"
#include "../framework/DSP/Drums/DrumsMachine.h"

class Rhythm
{
private:
	typedef DrumsMachine<SampleType, SAMPLE_RATE>::Notes Notes;

public:
	enum class Presets
	{
		QuarterNote1 = 0
	};

public:
	struct Data
	{
	public:
		Data(void)
			: IsEnabled(true),
			  Preset(Presets::QuarterNote1),
			  BeatsPerMinute(60),
			  Volume(1)
		{
		}

		bool IsEnabled;
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
	}

	void Update(void)
	{
		if (!m_Data.IsEnabled)
			return;

		m_DrumsMachine.Update();
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] = Math::Lerp(Buffer[i], m_DrumsMachine.Process(), m_Data.Volume);
	}

	void SetData(const Data &Data)
	{
		m_Data = Data;

		// static Notes QuarterNote1[4];
		// QuarterNote1[0] = Notes::Kick | Notes::Snare;
		// QuarterNote1[1] = Notes::Snare;
		// QuarterNote1[2] = Notes::Kick | Notes::Snare;
		// QuarterNote1[3] = Notes::Snare;

		static Notes QuarterNote1[4];
		QuarterNote1[0] = Notes::Kick;
		QuarterNote1[1] = Notes::Kick;
		QuarterNote1[2] = Notes::Kick;
		QuarterNote1[3] = Notes::Kick;

		switch (m_Data.Preset)
		{
		case Presets::QuarterNote1:
			m_DrumsMachine.SetNotes(QuarterNote1, 4);
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