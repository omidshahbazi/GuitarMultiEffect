#pragma once
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
		const Font &PRESET_NUMBER_TEXT_FONT = FONT_64;
		const Color &PRESET_NUMBER_TEXT_COLOR = COLOR_BLACK;
		const Font &PRESET_NAME_TEXT_FONT = FONT_40;
		const Color &PRESET_NAME_TEXT_COLOR = COLOR_BLACK;

		Screen::Draw(Canvas);

		const Preset::Data &presetData = GetPresetManager()->GetSelectedPreset()->GetData();

		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "PLAY", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, GetPresetVolume().c_str(), HEADER_DEFAULT_RIGHT_TEXT_FONT, HEADER_DEFAULT_RIGHT_TEXT_COLOR);

		auto &canvasDimension = Canvas.GetDimension();

		Rect rect;
		rect.Position = {0, HEADER_HEIGTH + (SPLITTER_THICKNESS * 10)};
		rect.Dimension = {canvasDimension.X, canvasDimension.Y - rect.Position.Y};
		Canvas.DrawFilledRectangle(rect, PRESET_BOX_COLOR);

		rect.Dimension.Y *= 0.5;
		DrawStringJustified(Canvas, rect, GetPresetNumber().c_str(), PRESET_NUMBER_TEXT_FONT, PRESET_NUMBER_TEXT_COLOR);

		rect.Position.Y += rect.Dimension.Y;
		DrawStringJustified(Canvas, rect, presetData.Name, PRESET_NAME_TEXT_FONT, PRESET_NAME_TEXT_COLOR);
	}
};

#endif