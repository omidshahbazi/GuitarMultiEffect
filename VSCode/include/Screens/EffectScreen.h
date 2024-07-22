#pragma once
#ifndef EFFECT_SCREEN_H
#define EFFECT_SCREEN_H

#include "ParameterScreen.h"

class EffectScreen : public ParameterScreen
{
public:
	EffectScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: ParameterScreen(PresetManager, ControlManager)
	{
	}

protected:
	cstr GetTitle(void) const override
	{
		auto preset = GetPresetManager()->GetSelectedPreset();
		uint8 index = preset->GetData().SelectedPointerIndex;

		ASSERT(index < Preset::EFFECT_COUNT, "Invalid SelectedPointerIndex");

		return preset->GetEffects()[index]->GetName();
	}

	void OnChoiceDataChanged(void) override
	{
		GetPresetManager()->GetSelectedPreset()->UpdateData();

		RefreshData();
	}

	void OnValueDataChanged(void) override
	{
		GetPresetManager()->GetSelectedPreset()->UpdateData();
	}

	void OnRefreshData(void) override
	{
		auto &presetData = GetPresetManager()->GetSelectedPreset()->GetData();

		ASSERT(presetData.SelectedPointerIndex < Preset::EFFECT_COUNT, "Invalid SelectedPointerIndex");

		auto effectData = presetData.EffectsData[presetData.SelectedPointerIndex];

#define ADD_CHOICE_DATA_2(effectName, parameterName, item1, item2) AddChoiceData(reinterpret_cast<int32 *>(&presetData.effectName##Data.parameterName), item1, item2)
#define ADD_CHOICE_DATA_3(effectName, parameterName, item1, item2, item3) AddChoiceData(reinterpret_cast<int32 *>(&presetData.effectName##Data.parameterName), item1, item2, item3)
#define ADD_CHOICE_DATA_4(effectName, parameterName, item1, item2, item3, item4) AddChoiceData(reinterpret_cast<int32 *>(&presetData.effectName##Data.parameterName), item1, item2, item3, item4)
#define ADD_CHOICE_DATA_5(effectName, parameterName, item1, item2, item3, item4, item5) AddChoiceData(reinterpret_cast<int32 *>(&presetData.effectName##Data.parameterName), item1, item2, item3, item4, item5)

#define ADD_VALUE_DATA(effectName, parameterName, minValue, maxValue, displayMultiplier, asInteger, title) AddValueData(&presetData.effectName##Data.parameterName, title, minValue, maxValue, displayMultiplier, asInteger)
#define ADD_VALUE_DATA_SPEED(effectName, parameterName, minValue, maxValue, displayMultiplier, asInteger, speed, title) AddValueData(&presetData.effectName##Data.parameterName, title, minValue, maxValue, displayMultiplier, asInteger, speed)

		if (false)
		{
		}
#ifdef ADD_FX_EFFECT
		else if (effectData->Index == presetData.FXData.Index)
		{
			ADD_CHOICE_DATA_2(FX, Type, "AUTO WAH", "CRY BABY");

			if (presetData.FXData.Type == FXEffect::Data::Types::CryBaby)
				ADD_VALUE_DATA(FX, WahRatio, 0, 1, 100, true, "RATIO");
		}
#endif
#ifdef ADD_DS_EFFECT
		else if (effectData->Index == presetData.DsData.Index)
		{
			ADD_CHOICE_DATA_3(Ds, Type, "OVERDRIVE", "DISTORTION", "FUZZ");

			if (presetData.DsData.Type == DsEffect::Data::Types::Overdrive)
			{
				ADD_VALUE_DATA(Ds, OverdriveDrive, 0, 1, 100, true, "DRIVE");
				ADD_VALUE_DATA(Ds, OverdriveGain, -6, 6, 1, true, "GAIN");
			}
			else if (presetData.DsData.Type == DsEffect::Data::Types::Distortion)
			{
				ADD_VALUE_DATA(Ds, DistortionLevel, 0, 1, 100, true, "LEVEL");
				ADD_VALUE_DATA(Ds, DistortionGain, -20, 10, 1, true, "GAIN");
			}
			else if (presetData.DsData.Type == DsEffect::Data::Types::Fuzz)
			{
				ADD_VALUE_DATA(Ds, FuzzTone, 0, 1, 100, true, "TONE");
				ADD_VALUE_DATA(Ds, FuzzLevel, 0, 1, 100, true, "LEVEL");
				ADD_VALUE_DATA(Ds, FuzzWet, 0, 1, 100, true, "FUZZ");
			}
		}
#endif
#ifdef ADD_AMP_EFFECT
		else if (effectData->Index == presetData.AmpData.Index)
		{
			ADD_VALUE_DATA(Amp, Gain, -20, 40, 1, true, "GAIN");
			ADD_VALUE_DATA(Amp, LowTone, -20, 20, 1, true, "LOW");
			ADD_VALUE_DATA(Amp, MidTone, -20, 20, 1, true, "MID");
			ADD_VALUE_DATA(Amp, HighTone, -20, 20, 1, true, "HIGH");
			ADD_VALUE_DATA(Amp, PresenceRatio, 0, 1, 100, true, "PRESENCE");
			ADD_VALUE_DATA(Amp, Master, 0, 1, 100, true, "Master");
		}
#endif
#ifdef ADD_EQ_EFFECT
		else if (effectData->Index == presetData.EqData.Index)
		{
			ADD_CHOICE_DATA_5(Eq, Type, "BOOSTING", "BOOSTING ALT.", "CRYING CHICKEN", "CRYING CHICK. VOC.", "CUSTOM");

			if (presetData.EqData.Type == EqEffect::Data::Types::Custom)
			{
				ADD_VALUE_DATA(Eq, Gain100, -15, 15, 1, true, "100HZ");
				ADD_VALUE_DATA(Eq, Gain200, -15, 15, 1, true, "200HZ");
				ADD_VALUE_DATA(Eq, Gain400, -15, 15, 1, true, "400HZ");
				ADD_VALUE_DATA(Eq, Gain800, -15, 15, 1, true, "800HZ");
				ADD_VALUE_DATA(Eq, Gain1_6K, -15, 15, 1, true, "1.6KHZ");
				ADD_VALUE_DATA(Eq, Gain3_2K, -15, 15, 1, true, "3.2KHZ");
				ADD_VALUE_DATA(Eq, Gain6_4K, -15, 15, 1, true, "6.4KHZ");
				ADD_VALUE_DATA(Eq, Gain, -15, 15, 1, true, "Gain");
			}
		}
#endif
#ifdef ADD_MOD_EFFECT
		else if (effectData->Index == presetData.ModData.Index)
		{
			ADD_CHOICE_DATA_4(Mod, Type, "CHORUS", "FLANGER", "PHASER", "TREMOLO");

			if (presetData.ModData.Type == ModEffect::Data::Types::Chorus)
			{
				ADD_VALUE_DATA(Mod, ChorusDepth, 0, 100, 1, true, "DEPTH");
				ADD_VALUE_DATA_SPEED(Mod, ChorusRate, 0.5, 4, 1, false, 0.5, "RATE");
				ADD_VALUE_DATA(Mod, ChorusWetRate, 0, 1, 100, true, "WET");
			}
			else if (presetData.ModData.Type == ModEffect::Data::Types::Flanger)
			{
				ADD_VALUE_DATA(Mod, FlangerDepth, 0, 100, 1, true, "DEPTH");
				ADD_VALUE_DATA_SPEED(Mod, FlangerRate, 0.5, 4, 1, false, 0.5, "RATE");
				ADD_VALUE_DATA(Mod, FlangerFeedback, 0, 1, 100, true, "FEEDBACK");
				ADD_VALUE_DATA(Mod, FlangerWetRate, 0, 1, 100, true, "WET");
			}
			else if (presetData.ModData.Type == ModEffect::Data::Types::Phaser)
			{
				ADD_VALUE_DATA(Mod, PhaserDepth, 0, 100, 1, true, "DEPTH");
				ADD_VALUE_DATA_SPEED(Mod, PhaserRate, 0.5, 3.5, 1, false, 0.5, "RATE");
				ADD_VALUE_DATA(Mod, PhaserWetRate, 0, 1, 100, true, "WET");
			}
			else if (presetData.ModData.Type == ModEffect::Data::Types::Tremolo)
			{
				ADD_VALUE_DATA(Mod, TremoloDepth, 0, 1, 100, true, "DEPTH");
				ADD_VALUE_DATA(Mod, TremoloRate, 1, 25, 1, true, "RATE");
				ADD_VALUE_DATA(Mod, TremoloWetRate, 0, 1, 100, true, "WET");
			}
		}
#endif
#ifdef ADD_DEL_EFFECT
		else if (effectData->Index == presetData.DelData.Index)
		{
			ADD_CHOICE_DATA_2(Del, Type, "NORMAL", "REVERSE");

			ADD_VALUE_DATA_SPEED(Del, DelayTime, 0, DelEffect::Data::MAX_DELAY_TIME, 1, false, 0.1, "LENGTH");
			ADD_VALUE_DATA(Del, Feedback, 0, 1, 100, true, "FEEDBACK");
		}
#endif
#ifdef ADD_REV_EFFECT
		else if (effectData->Index == presetData.RevData.Index)
		{
			ADD_VALUE_DATA_SPEED(Rev, DelayTime, 0, RevEffect::Data::MAX_DELAY_TIME, 1, false, 0.1, "LENGTH");
			ADD_VALUE_DATA(Rev, Feedback, 0, 1, 100, true, "FEEDBACK");
			ADD_VALUE_DATA(Rev, WetRate, 0, 1, 100, true, "WET");
		}
#endif

#undef ADD_CHOICE_DATA_4
#undef ADD_CHOICE_DATA_3
#undef ADD_CHOICE_DATA_2
#undef ADD_VALUE_DATA
	}
};

#endif