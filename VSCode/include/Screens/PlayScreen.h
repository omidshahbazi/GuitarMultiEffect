#pragma once
#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include "Screen.h"

class PlayScreen : public Screen
{
public:
	void Draw(LCDCanvas &Canvas) override
	{
		const Point &canvasDim = Canvas.GetDimension();

		// Volume
		{
			Rect rect(10, 10, 60, 30);

			Canvas.DrawFilledRectangle(rect, PRESET_VOLUME_BOX_COLOR);
			Canvas.DrawString(rect.Position, "100%", PRESET_VOLUME_TEXT_FONT, PRESET_INFO_TEXT_COLOR);
		}

		// Number/Name
		{
			Rect rect(10, 45, 300, 185);

			Canvas.DrawFilledRectangle(rect, PRESET_INFO_BOX_COLOR);

			Rect textRect = rect;

			Point textDim = Canvas.MeasureStringDimension("P 171", PRESET_INFO_TEXT_FONT);
			textRect.Position.X = (canvasDim.X * 0.5) - (textDim.X * 0.5);
			textRect.Position.Y += 30;
			Canvas.DrawString(textRect.Position, "P 171", PRESET_INFO_TEXT_FONT, PRESET_INFO_TEXT_COLOR);

			textDim = Canvas.MeasureStringDimension("SCOTTY 01", PRESET_INFO_TEXT_FONT);
			textRect.Position.X = (canvasDim.X * 0.5) - (textDim.X * 0.5);
			textRect.Position.Y += 30;
			Canvas.DrawString(textRect.Position, "SCOTTY 01", PRESET_INFO_TEXT_FONT, PRESET_INFO_TEXT_COLOR);
		}
	}
};

#endif