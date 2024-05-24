#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include "Screen.h"

class PlayScreen : public Screen
{
public:
	PlayScreen(PresetManager *PresetManager)
		: Screen(PresetManager)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		const uint8 HEADER_HEIGTH = 30;

		const Color &PRESET_BOX_COLOR = COLOR_GREEN;
		const Font &PRESET_TEXT_FONT = FONT_20;
		const Color &PRESET_TEXT_COLOR = COLOR_DARK_BLUE;

		Screen::Draw(Canvas);

		const uint8 presetIndex = GetPresetManager()->GetSelectedPresetIndex();
		const Preset::Data &presetData = GetPresetManager()->GetSelectedPreset()->GetData();

		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "SAVE", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});
	}
};

#endif
#pragma GCC pop_options