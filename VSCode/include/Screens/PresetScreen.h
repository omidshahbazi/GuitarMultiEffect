#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef PRESET_SCREEN_H
#define PRESET_SCREEN_H

#include "Screen.h"
#include <string>

class PresetScreen : public Screen
{
public:
	PresetScreen(PresetManager *PresetManager)
		: Screen(PresetManager)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		const Color &PRESET_INDEX_BOX_COLOR = HEADER_DEFAULT_LEFT_BOX_COLOR;
		const Font &PRESET_INDEX_TEXT_FONT = HEADER_DEFAULT_LEFT_TEXT_FONT;
		const Color &PRESET_INDEX_TEXT_COLOR = HEADER_DEFAULT_LEFT_TEXT_COLOR;

		const Color &PRESET_NAME_BOX_COLOR = HEADER_DEFAULT_MIDDLE_BOX_COLOR;
		const Font &PRESET_NAME_TEXT_FONT = HEADER_DEFAULT_MIDDLE_TEXT_FONT;
		const Color &PRESET_NAME_TEXT_COLOR = HEADER_DEFAULT_MIDDLE_TEXT_COLOR;

		const Color &PRESET_VOLUME_BOX_COLOR = HEADER_DEFAULT_RIGHT_BOX_COLOR;
		const Font &PRESET_VOLUME_TEXT_FONT = HEADER_DEFAULT_RIGHT_TEXT_FONT;
		const Color &PRESET_VOLUME_TEXT_COLOR = HEADER_DEFAULT_RIGHT_TEXT_COLOR;

		Screen::Draw(Canvas);

		const uint8 presetIndex = GetPresetManager()->GetSelectedPresetIndex();
		const Preset::Data &presetData = GetPresetManager()->GetSelectedPreset()->GetData();

		DrawHeader(Canvas, DEFAULT_HEADER_HEIGHT,
				   PRESET_INDEX_BOX_COLOR, GetPresetNumber().c_str(), PRESET_INDEX_TEXT_FONT, PRESET_INDEX_TEXT_COLOR,
				   PRESET_NAME_BOX_COLOR, presetData.Name, PRESET_NAME_TEXT_FONT, PRESET_NAME_TEXT_COLOR,
				   PRESET_VOLUME_BOX_COLOR, GetPresetVolume().c_str(), PRESET_VOLUME_TEXT_FONT, PRESET_VOLUME_TEXT_COLOR);
	}
};

#endif
#pragma GCC pop_options