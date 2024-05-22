#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef PRESET_H
#define PRESET_H

#include "../Common.h"
#include "../Effects/EffectManager.h"
#include "../framework/DSP/Memory.h"
#include "../framework/DSP/Math.h"

class Preset
{
public:
	static const uint8 MAX_NAME_LENGTH = 10;
	static const uint8 MAX_EFFECT_COUNT = 1;

	struct Data
	{
	public:
		char Name[MAX_NAME_LENGTH + 1];
		float Volume;

		OverdriveEffect::Data OverdriveData;
	};

public:
	Preset(void)
		: m_Data{},
		  m_EffectCount(0)
	{
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		for (uint8 i = 0; i < m_EffectCount; ++i)
			m_Effects[i]->Apply(Buffer, Count);

		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] *= m_Data.Volume;
	}

	void SetData(const Data &Data)
	{
		m_Data = Data;

		// TODO: set each one

		OverdriveEffect *overdrive = EffectManager::GetEffect<OverdriveEffect>(EffectManager::Types::Overdrive);
		overdrive->SetData(m_Data.OverdriveData);

		m_Effects[m_EffectCount++] = overdrive;
	}

	const Data &GetData(void) const
	{
		return m_Data;
	}

	void SetName(const char *const Value)
	{
		SetName(m_Data, Value);
	}

	void SetVolume(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Data.Volume = Value;
	}

	static void SetName(Data &Data, const char *const Value)
	{
		uint8 length = GetStringLength(Value);

		ASSERT(length <= MAX_NAME_LENGTH, "Value's lenght is out of bound");

		Memory::Copy(Value, Data.Name, length);
		Data.Name[length] = '\0';
	}

private:
	Data m_Data;

	Effect *m_Effects[MAX_EFFECT_COUNT];
	uint8 m_EffectCount;
};

#endif
#pragma GCC pop_options