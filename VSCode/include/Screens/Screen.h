#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include "../Presets/PresetManager.h"
#include "../ControlManager.h"
#include "../framework/LCDCanvas.h"
#include <string>

const Color COLOR_BLACK = {};
const Color COLOR_GRAY = {128, 128, 128};
const Color COLOR_WHITE = {255, 255, 255};
const Color COLOR_RED = {180, 23, 42};
const Color COLOR_GREEN = {122, 159, 55};
const Color COLOR_LIGHT_PURPLE = {144, 104, 227};
const Color COLOR_DARK_PURPLE = {118, 41, 255};
const Color COLOR_YELLOW = {255, 245, 130};
const Color COLOR_LIGHT_BLUE = {160, 187, 216};
const Color COLOR_DARK_BLUE = {41, 57, 106};

const Font &FONT_16 = Font_DUBAI_BOLD_16;
const Font &FONT_20 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 1.25};
const Font &FONT_24 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 1.5};
const Font &FONT_30 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 1.875};
const Font &FONT_32 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 2};
const Font &FONT_40 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 2.5};
const Font &FONT_64 = {Font_DUBAI_BOLD_16.Width, Font_DUBAI_BOLD_16.Height, Font_DUBAI_BOLD_16.Data, 4};

const Color BACKGROUND_FILL_COLOR = COLOR_BLACK;

const uint16 DEFAULT_HEADER_HEIGTH = 50;
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

const uint16 HEADER_LEFT_PART_WIDTH = 50;
const uint16 HEADER_MIDDLE_PART_WIDTH = 170;
const uint8 HEADER_TRI_EDGE_WIDTH = 20;

class ScreenManager;

class Screen
{
	friend class ScreenManager;

public:
	typedef ContextCallback<void, Screens> SwitchScreenEventHandler;
	typedef ContextCallback<void> ClearScreensHistoryEventHandler;

public:
	Screen(PresetManager *PresetManager, ControlManager *ControlManager)
		: m_PresetManager(PresetManager),
		  m_ControlManager(ControlManager),
		  m_IsDirty(true)
	{
	}

	virtual void Update(void)
	{
	}

	void Render(LCDCanvas &Canvas)
	{
		if (!m_IsDirty)
			return;
		m_IsDirty = false;

		Draw(Canvas);
	}

	virtual void ProcessAudioBuffer(const SampleType *const InputBuffer, const SampleType *const OutputBuffer, uint8 Count)
	{
	}

protected:
	virtual void Draw(LCDCanvas &Canvas)
	{
		Canvas.Clear(BACKGROUND_FILL_COLOR);
	}

	virtual void Activate(void)
	{
		MarkAsDirty();
	}

	virtual void Deactivate(void)
	{
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
		return ("VOL" + std::to_string((uint8)(GetPresetManager()->GetSelectedPreset()->GetData().OutputVolume * 100)));
	}

	PresetManager *GetPresetManager(void) const
	{
		return m_PresetManager;
	}

	ControlManager *GetControlManager(void) const
	{
		return m_ControlManager;
	}

	void SetOnSwitchScreen(SwitchScreenEventHandler Listener)
	{
		m_OnSwitchScreen = Listener;
	}

	void SetOnClearScreensHistory(ClearScreensHistoryEventHandler Listener)
	{
		m_OnClearScreensHistory = Listener;
	}

	void SwitchScreen(Screens Screen)
	{
		m_OnSwitchScreen(Screen);
	}

	void ClearScreensHistory(void)
	{
		m_OnClearScreensHistory();
	}

	static void DrawStringJustified(LCDCanvas &Canvas, Rect Rect, cstr Text, const Font &Font, Color Color)
	{
		DrawStringJustified(Canvas, Rect, Text, GetStringLength(Text), Font, Color);
	}

	static void DrawStringJustified(LCDCanvas &Canvas, Rect Rect, cstr Text, uint16 Length, const Font &Font, Color Color)
	{
		Point textDim = Canvas.MeasureStringDimension(Text, Length, Font);

		Rect.Position.X += (Rect.Dimension.X * 0.5) - (textDim.X * 0.5);
		Rect.Position.Y += (Rect.Dimension.Y * 0.5) - (textDim.Y * 0.5);

		Canvas.DrawString(Rect.Position, Text, Length, Font, Color);
	}

	static void DrawHeader(LCDCanvas &Canvas, uint16 Height,
						   Color LeftPartColor, cstr LeftPartText, const Font &LeftPartTextFont, Color LeftPartTextColor,
						   Color MiddlePartColor, cstr MiddlePartText, const Font &MiddlePartTextFont, Color MiddlePartTextColor,
						   Color RightPartColor, cstr RightPartText, const Font &RightPartTextFont, Color RightPartTextColor)
	{

		Rect rect;

		// Left Part
		{
			rect = {0, 0, HEADER_LEFT_PART_WIDTH, Height};

			Canvas.DrawFilledRectangle(rect, LeftPartColor);

			if (LeftPartText != nullptr)
				DrawStringJustified(Canvas, rect, LeftPartText, LeftPartTextFont, LeftPartTextColor);
		}

		// Middle Part
		{
			rect.Position.X += HEADER_LEFT_PART_WIDTH + SPLITTER_THICKNESS;
			rect.Dimension.X = HEADER_MIDDLE_PART_WIDTH;

			Canvas.DrawFilledParallelogram(
				rect.Position,
				{rect.Position.X, rect.Position.Y + rect.Dimension.Y},
				{rect.Position.X + rect.Dimension.X + HEADER_TRI_EDGE_WIDTH, rect.Position.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y + rect.Dimension.Y},
				MiddlePartColor);

			if (MiddlePartText != nullptr)
				DrawStringJustified(Canvas, rect, MiddlePartText, MiddlePartTextFont, MiddlePartTextColor);
		}

		// Right Part
		{
			rect.Position.X += rect.Dimension.X + HEADER_TRI_EDGE_WIDTH + (SPLITTER_THICKNESS * 2);
			rect.Dimension.X = (Canvas.GetDimension().X - rect.Position.X);

			Canvas.DrawFilledParallelogram(
				rect.Position,
				{rect.Position.X - HEADER_TRI_EDGE_WIDTH, rect.Position.Y + rect.Dimension.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y},
				{rect.Position.X + rect.Dimension.X, rect.Position.Y + rect.Dimension.Y},
				RightPartColor);

			if (RightPartText != nullptr)
				DrawStringJustified(Canvas, rect, RightPartText, RightPartTextFont, RightPartTextColor);
		}
	}

	static void DrawProgressBar(LCDCanvas &Canvas, Rect Rect, Color BorderColor, cstr Title, Color TitleColor, float Level, Color LevelColor, bool DrawMaxLevel, float MaxLevel, Color MaxLevelColor)
	{
		::Rect valueRect = Rect;
		valueRect.Dimension.X *= Level;
		Canvas.DrawFilledRectangle(valueRect, LevelColor);

		DrawStringJustified(Canvas, Rect, Title, FONT_20, TitleColor);

		if (DrawMaxLevel)
		{
			uint16 x = Rect.Position.X + (Rect.Dimension.X * MaxLevel);
			Canvas.DrawLine({x, Rect.Position.Y}, {x, Rect.Position.Y + Rect.Dimension.Y}, MaxLevelColor);
		}

		Canvas.DrawRectangle(Rect, BorderColor);
	}

private:
	PresetManager *m_PresetManager;
	ControlManager *m_ControlManager;
	bool m_IsDirty;
	SwitchScreenEventHandler m_OnSwitchScreen;
	ClearScreensHistoryEventHandler m_OnClearScreensHistory;
};

#endif