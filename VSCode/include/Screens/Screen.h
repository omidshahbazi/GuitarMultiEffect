#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include "../Presets/PresetManager.h"
#include "../framework/LCDCanvas.h"

const Color COLOR_BLACK = {};
const Color COLOR_GREEN = {122, 159, 55};
const Color COLOR_LIGHT_PURPLE = {144, 104, 227};
const Color COLOR_DARK_PURPLE = {118, 41, 255};
const Color COLOR_YELLOW = {255, 245, 130};
const Color COLOR_LIGHT_BLUE = {160, 187, 216};
const Color COLOR_DARK_BLUE = {41, 57, 106};

const Font &FONT_16 = Font_DUBAI_BOLD_16;
const Font &FONT_20 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 1.25};
const Font &FONT_32 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 2};
const Font &FONT_40 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 2.5};
const Font &FONT_64 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 4};

const Color BACKGROUND_FILL_COLOR = COLOR_BLACK;

const uint16 DEFAULT_HEADER_HEIGHT = 50;
const uint8 SPLITTER_THICKNESS = 2;

const Color &HEADER_DEFAULT_LEFT_BOX_COLOR = COLOR_DARK_BLUE;
const Font &HEADER_DEFAULT_LEFT_TEXT_FONT = FONT_20;
const Color &HEADER_DEFAULT_LEFT_TEXT_COLOR = COLOR_LIGHT_BLUE;

const Color &HEADER_DEFAULT_MIDDLE_BOX_COLOR = COLOR_LIGHT_BLUE;
const Font &HEADER_DEFAULT_MIDDLE_TEXT_FONT = FONT_20;
const Color &HEADER_DEFAULT_MIDDLE_TEXT_COLOR = COLOR_DARK_BLUE;

const Color &HEADER_DEFAULT_RIGHT_BOX_COLOR = COLOR_LIGHT_PURPLE;
const Font &HEADER_DEFAULT_RIGHT_TEXT_FONT = FONT_16;
const Color &HEADER_DEFAULT_RIGHT_TEXT_COLOR = COLOR_DARK_BLUE;

class ScreenManager;

class Screen
{
	friend class ScreenManager;

public:
	Screen(PresetManager *PresetManager)
		: m_PresetManager(PresetManager),
		  m_IsDirty(true)
	{
	}

	void Render(LCDCanvas &Canvas)
	{
		if (!m_IsDirty)
			return;

		Draw(Canvas);

		m_IsDirty = false;
	}

	PresetManager *GetPresetManager(void) const
	{
		return m_PresetManager;
	}

protected:
	virtual void Draw(LCDCanvas &Canvas)
	{
		Canvas.Clear(BACKGROUND_FILL_COLOR);
	}

	void MarkAsDirty(void)
	{
		m_IsDirty = true;
	}

	std::string GetPresetNumber(void) const
	{
		return ("P" + std::to_string(GetPresetManager()->GetSelectedPresetIndex() + 1));
	}

	std::string GetPresetVolume(void) const
	{
		return ("VOL" + std::to_string((uint8)(GetPresetManager()->GetSelectedPreset()->GetData().Volume * 100)));
	}

	static void DrawStringJustified(LCDCanvas &Canvas, Rect Rect, cstr Text, const Font &Font, Color Color)
	{
		uint8 len = GetStringLength(Text);

		Point textDim = Canvas.MeasureStringDimension(Text, len, Font);

		Rect.Position.X += (Rect.Dimension.X * 0.5) - (textDim.X * 0.5);
		Rect.Position.Y += (Rect.Dimension.Y * 0.5) - (textDim.Y * 0.5);

		Canvas.DrawString(Rect.Position, Text, len, Font, Color);
	}

	static void DrawHeader(LCDCanvas &Canvas, uint16 Height,
						   Color LeftPartColor, cstr LeftPartText, const Font &LeftPartTextFont, Color LeftPartTextColor,
						   Color MiddlePartColor, cstr MiddlePartText, const Font &MiddlePartTextFont, Color MiddlePartTextColor,
						   Color RightPartColor, cstr RightPartText, const Font &RightPartTextFont, Color RightPartTextColor)
	{
		const uint16 LEFT_PART_WIDTH = 50;
		const uint16 MIDDLE_PART_WIDTH = 170;
		const uint8 TRI_EDGE_WIDTH = 20;

		Rect rect;

		// Left Part
		{
			rect = {0, 0, LEFT_PART_WIDTH, Height};

			Canvas.DrawFilledRectangle(rect, LeftPartColor);

			if (LeftPartText != nullptr)
				DrawStringJustified(Canvas, rect, LeftPartText, LeftPartTextFont, LeftPartTextColor);
		}

		// Middle Part
		{
			rect.Position.X += LEFT_PART_WIDTH + SPLITTER_THICKNESS;
			rect.Dimension.X = MIDDLE_PART_WIDTH;

			Canvas.DrawFilledParallelogram(
				rect.Position,
				{rect.Position.X, rect.Position.Y + rect.Dimension.Y},
				{rect.Position.X + rect.Dimension.X + TRI_EDGE_WIDTH, rect.Position.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y + rect.Dimension.Y},
				MiddlePartColor);

			if (MiddlePartText != nullptr)
				DrawStringJustified(Canvas, rect, MiddlePartText, MiddlePartTextFont, MiddlePartTextColor);
		}

		// Right Part
		{
			rect.Position.X += rect.Dimension.X + TRI_EDGE_WIDTH + (SPLITTER_THICKNESS * 2);
			rect.Dimension.X = (Canvas.GetDimension().X - rect.Position.X);

			Canvas.DrawFilledParallelogram(
				rect.Position,
				{rect.Position.X - TRI_EDGE_WIDTH, rect.Position.Y + rect.Dimension.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y + rect.Dimension.Y},
				RightPartColor);

			if (RightPartText != nullptr)
				DrawStringJustified(Canvas, rect, RightPartText, RightPartTextFont, RightPartTextColor);
		}
	}

private:
	PresetManager *m_PresetManager;
	bool m_IsDirty;
};

#endif