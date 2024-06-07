#pragma once
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
		Data(void)
			: Volume(1),
			  EffectsData{}
		{
			Preset::SetName(*this, "EMPTY");
		}

		char Name[MAX_NAME_LENGTH + 1];
		float Volume;

		FXEffect::Data FXData;
		DsEffect::Data DsData;
		AmpEffect::Data AmpData;
		EqEffect::Data EqData;
		ModEffect::Data ModData;
		DelEffect::Data DelData;
		RevEffect::Data RevData;

		Effect::Data *EffectsData[EFFECT_COUNT];
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

		SET_DATA(FX);
		SET_DATA(Ds);
		SET_DATA(Amp);
		SET_DATA(Eq);
		SET_DATA(Mod);
		SET_DATA(Del);
		SET_DATA(Rev);

		for (uint8 i = 0; i < EFFECT_COUNT; ++i)
		{
			if (i == m_Data.FXData.Index)
				m_Data.EffectsData[i] = &m_Data.FXData;
			else if (i == m_Data.DsData.Index)
				m_Data.EffectsData[i] = &m_Data.DsData;
			else if (i == m_Data.AmpData.Index)
				m_Data.EffectsData[i] = &m_Data.AmpData;
			else if (i == m_Data.EqData.Index)
				m_Data.EffectsData[i] = &m_Data.EqData;
			else if (i == m_Data.ModData.Index)
				m_Data.EffectsData[i] = &m_Data.ModData;
			else if (i == m_Data.DelData.Index)
				m_Data.EffectsData[i] = &m_Data.DelData;
			else if (i == m_Data.RevData.Index)
				m_Data.EffectsData[i] = &m_Data.RevData;
		}

#undef SET_DATA
	}

	Data &GetData(void)
	{
		return m_Data;
	}

	void UpdateData(void)
	{
		SetData(GetData());
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

	Effect **GetEffects(void)
	{
		return m_Effects;
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