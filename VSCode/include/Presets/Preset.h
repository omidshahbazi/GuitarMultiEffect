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
	static constexpr uint8 MAX_NAME_LENGTH = 10;
	static constexpr uint8 EFFECT_COUNT = (uint8)EffectManager::Types::COUNT;

	struct Data
	{
	public:
		char Name[MAX_NAME_LENGTH + 1];
		float Volume;

		OverdriveEffect::Data OverdriveData;
		ReverbEffect::Data ReverbData;
	};

public:
	Preset(void)
		: m_Data{},
		  m_Effects{}
	{
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		for (uint8 i = 0; i < EFFECT_COUNT; ++i)
			m_Effects[i]->Apply(Buffer, Count);

		for (uint8 i = 0; i < Count; ++i)
			Buffer[i] *= m_Data.Volume;
	}

	void SetData(const Data &Data)
	{
#define SET_DATA(EffectName)                                                                                         \
	{                                                                                                                \
		EffectName##Effect *effect = EffectManager::GetEffect<EffectName##Effect>(EffectManager::Types::EffectName); \
		effect->SetData(m_Data.EffectName##Data);                                                                    \
		m_Effects[m_Data.EffectName##Data.Index] = effect;                                                           \
	}

		m_Data = Data;

		SET_DATA(Overdrive);
		SET_DATA(Reverb);

#undef SET_DATA
	}

	const Data &GetData(void) const
	{
		return m_Data;
	}

	void SetName(cstr Value)
	{
		SetName(m_Data, Value);
	}

	void SetVolume(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Data.Volume = Value;
	}

	static void SetName(Data &Data, cstr Value)
	{
		uint8 length = GetStringLength(Value);

		ASSERT(length <= MAX_NAME_LENGTH, "Value's lenght is out of bound");

		Memory::Copy(Value, Data.Name, length);
		Data.Name[length] = '\0';
	}

private:
	Data m_Data;
	Effect *m_Effects[EFFECT_COUNT];
};

#endif
#pragma GCC pop_options