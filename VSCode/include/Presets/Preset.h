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
			: InputGain(0),
			  OutputGain(0),
			  OutputVolume(1),
			  EffectsData{},
			  SelectedPointerIndex(0)
		{
			Preset::SetName(*this, "EMPTY");

			uint8 effectIndex = 0;

#ifdef ADD_FX_EFFECT
			FXData.Index = effectIndex++;
#endif
#ifdef ADD_DS_EFFECT
			DsData.Index = effectIndex++;
#endif
#ifdef ADD_AMP_EFFECT
			AmpData.Index = effectIndex++;
#endif
#ifdef ADD_EQ_EFFECT
			EqData.Index = effectIndex++;
#endif
#ifdef ADD_MOD_EFFECT
			ModData.Index = effectIndex++;
#endif
#ifdef ADD_DEL_EFFECT
			DelData.Index = effectIndex++;
#endif
#ifdef ADD_REV_EFFECT
			RevData.Index = effectIndex++;
#endif
		}

		char Name[MAX_NAME_LENGTH + 1];

		//[-20dB, 40dB]
		float InputGain;
		//[-30dB, 40dB]
		float OutputGain;
		//[0, 1]
		float OutputVolume;

#ifdef ADD_FX_EFFECT
		FXEffect::Data FXData;
#endif
#ifdef ADD_DS_EFFECT
		DsEffect::Data DsData;
#endif
#ifdef ADD_AMP_EFFECT
		AmpEffect::Data AmpData;
#endif
#ifdef ADD_EQ_EFFECT
		EqEffect::Data EqData;
#endif
#ifdef ADD_MOD_EFFECT
		ModEffect::Data ModData;
#endif
#ifdef ADD_DEL_EFFECT
		DelEffect::Data DelData;
#endif
#ifdef ADD_REV_EFFECT
		RevEffect::Data RevData;
#endif

		Effect::Data *EffectsData[EFFECT_COUNT];
		uint8 SelectedPointerIndex;
	};

public:
	Preset(void)
		: m_Data{},
		  m_Effects{}
	{
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		for (uint8 i = 0; i < Count; ++i)
		{
			SampleType sample = Buffer[i];
			sample *= Math::dbToMultiplier(m_Data.InputGain);
			sample = Math::Clamp(sample, -1, 1);
			Buffer[i] = sample;
		}

		for (uint8 i = 0; i < EFFECT_COUNT; ++i)
			m_Effects[i]->Apply(Buffer, Count);

		for (uint8 i = 0; i < Count; ++i)
		{
			SampleType sample = Buffer[i];
			sample *= Math::dbToMultiplier(m_Data.OutputGain);
			sample = Math::Clamp(sample, -1, 1);
			sample *= m_Data.OutputVolume;
			Buffer[i] = sample;
		}
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

#ifdef ADD_FX_EFFECT
		SET_DATA(FX);
#endif
#ifdef ADD_DS_EFFECT
		SET_DATA(Ds);
#endif
#ifdef ADD_AMP_EFFECT
		SET_DATA(Amp);
#endif
#ifdef ADD_EQ_EFFECT
		SET_DATA(Eq);
#endif
#ifdef ADD_MOD_EFFECT
		SET_DATA(Mod);
#endif
#ifdef ADD_DEL_EFFECT
		SET_DATA(Del);
#endif
#ifdef ADD_REV_EFFECT
		SET_DATA(Rev);
#endif

		for (uint8 i = 0; i < EFFECT_COUNT; ++i)
		{
			if (false)
			{
			}
#ifdef ADD_FX_EFFECT
			else if (i == m_Data.FXData.Index)
				m_Data.EffectsData[i] = &m_Data.FXData;
#endif
#ifdef ADD_DS_EFFECT
			else if (i == m_Data.DsData.Index)
				m_Data.EffectsData[i] = &m_Data.DsData;
#endif
#ifdef ADD_AMP_EFFECT
			else if (i == m_Data.AmpData.Index)
				m_Data.EffectsData[i] = &m_Data.AmpData;
#endif
#ifdef ADD_EQ_EFFECT
			else if (i == m_Data.EqData.Index)
				m_Data.EffectsData[i] = &m_Data.EqData;
#endif
#ifdef ADD_MOD_EFFECT
			else if (i == m_Data.ModData.Index)
				m_Data.EffectsData[i] = &m_Data.ModData;
#endif
#ifdef ADD_DEL_EFFECT
			else if (i == m_Data.DelData.Index)
				m_Data.EffectsData[i] = &m_Data.DelData;
#endif
#ifdef ADD_REV_EFFECT
			else if (i == m_Data.RevData.Index)
				m_Data.EffectsData[i] = &m_Data.RevData;
#endif
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