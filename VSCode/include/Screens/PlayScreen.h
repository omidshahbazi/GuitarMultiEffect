#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include "Screen.h"
#include <string>

class PlayScreen : public Screen
{
public:
	PlayScreen(PresetManager *PresetManager)
		: Screen(PresetManager)
	{
	}

	void Draw(LCDCanvas &Canvas) override
	{
		const Point &canvasDim = Canvas.GetDimension();

		const Preset::Data &data = GetPresetManager()->GetCurrentPreset()->GetData();

		// Volume
		{
			Rect rect(10, 10, 60, 30);

			auto value = std::to_string((int8)(data.Volume * 100));

			Canvas.DrawFilledRectangle(rect, PRESET_VOLUME_BOX_COLOR);
			Canvas.DrawString(rect.Position, value.c_str(), value.length(), PRESET_VOLUME_TEXT_FONT, PRESET_INFO_TEXT_COLOR);
		}

		// Number/Name
		{
			Rect rect(10, 45, 300, 185);

			Canvas.DrawFilledRectangle(rect, PRESET_INFO_BOX_COLOR);

			Rect textRect = rect;

			uint8 len = GetStringLength(data.Name);

			Point textDim = Canvas.MeasureStringDimension(data.Name, len, PRESET_INFO_TEXT_FONT);
			textRect.Position.X = (canvasDim.X * 0.5) - (textDim.X * 0.5);
			textRect.Position.Y += 30;
			Canvas.DrawString(textRect.Position, data.Name, len, PRESET_INFO_TEXT_FONT, PRESET_INFO_TEXT_COLOR);
		}
	}
};

#endif
#pragma GCC pop_options