#pragma once
#ifndef RHYTHM_SCREEN_H
#define RHYTHM_SCREEN_H

#include "ParameterScreen.h"

class RhythmScreen : public ParameterScreen
{
public:
	RhythmScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: ParameterScreen(PresetManager, ControlManager)
	{
	}

protected:
	cstr GetTitle(void) const override
	{
		return "RHYTHM";
	}

	void OnChoiceDataChanged(void) override
	{
		GetPresetManager()->GetRhythm()->UpdateData();

		RefreshData();
	}

	void OnValueDataChanged(void) override
	{
		GetPresetManager()->GetRhythm()->UpdateData();
	}

	void OnRefreshData(void) override
	{
		auto &data = GetPresetManager()->GetRhythm()->GetData();

		AddChoiceData(reinterpret_cast<int32 *>(&data.IsEnabled), "OFF", "ON");
		AddChoiceData(reinterpret_cast<int32 *>(&data.Preset), "HALF NOTE 1", "HALF NOTE 2", "HALF NOTE 3", "HALF NOTE 4", "QUARTER NOTE 1", "QUARTER NOTE 2", "QUARTER NOTE 3", "QUARTER NOTE 4");

		AddValueData(&data.BeatsPerMinute, "TEMPO", 20, 400, 1, true);
		AddValueData(&data.Volume, "VOL", 0, 1, 100, true);
	}
};

#endif