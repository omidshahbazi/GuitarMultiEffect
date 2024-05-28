#pragma once
#ifndef PRESET_SCREEN_H
#define PRESET_SCREEN_H

#include "Screen.h"
#include <string>

#pragma GCC pop_options

class PresetScreen : public Screen
{
private:
	enum class SelectionTypes
	{
		Name = 0,
		Volume,
		Effect
	};

public:
	PresetScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_SelectionType(SelectionTypes::Name),
		  m_SelectedEffectIndex(0)
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

		auto &canvasDimensions = Canvas.GetDimension();

		const Color LINE_COLOR = COLOR_LIGHT_BLUE;
		const Color ENABLED_EFFECT_COLOR = COLOR_DARK_BLUE;
		const Color DISABLED_EFFECT_COLOR = COLOR_GRAY;
		const Font &EFFECT_NAME_FONT = FONT_20;
		Color EFFECT_NAME_COLOR = COLOR_LIGHT_BLUE;
		const uint16 START_HEIGHT = DEFAULT_HEADER_HEIGHT + 50;
		const uint16 LINES_OFFSET = 70;
		const Point EFFECT_DIMENSIONS = {20, 30};
		const Point HALF_EFFECT_DIMENSIONS = {EFFECT_DIMENSIONS.X * 0.5, EFFECT_DIMENSIONS.Y * 0.5};
		const uint8 LINE_COUNT = 2;
		const uint8 MAX_EFFECT_PER_LINE = Math::Ceil(Preset::EFFECT_COUNT + 1 / (float)LINE_COUNT);
		const uint8 EFFECTS_OFFSET = canvasDimensions.X / MAX_EFFECT_PER_LINE;
		const uint8 HALF_EFFECTS_OFFSET = EFFECTS_OFFSET * 0.5;
		const uint8 SELECTION_SIGN_OFFSET = 8;

		Screen::Draw(Canvas);

		Preset *preset = GetPresetManager()->GetSelectedPreset();

		const Preset::Data &presetData = preset->GetData();
		DrawHeader(Canvas, DEFAULT_HEADER_HEIGHT,
				   PRESET_INDEX_BOX_COLOR, GetPresetNumber().c_str(), PRESET_INDEX_TEXT_FONT, PRESET_INDEX_TEXT_COLOR,
				   PRESET_NAME_BOX_COLOR, presetData.Name, PRESET_NAME_TEXT_FONT, PRESET_NAME_TEXT_COLOR,
				   PRESET_VOLUME_BOX_COLOR, GetPresetVolume().c_str(), PRESET_VOLUME_TEXT_FONT, PRESET_VOLUME_TEXT_COLOR);

		if (m_SelectionType == SelectionTypes::Name)
		{
			DrawSelectionSign(Canvas, {HEADER_LEFT_PART_WIDTH + (uint16)(HEADER_MIDDLE_PART_WIDTH * 0.5), DEFAULT_HEADER_HEIGHT + SELECTION_SIGN_OFFSET}, 1, COLOR_WHITE);
		}
		else if (m_SelectionType == SelectionTypes::Volume)
		{
			uint16 leftAndRightWidth = HEADER_LEFT_PART_WIDTH + HEADER_MIDDLE_PART_WIDTH;
			DrawSelectionSign(Canvas, {leftAndRightWidth + (uint16)((canvasDimensions.X - leftAndRightWidth) * 0.5), DEFAULT_HEADER_HEIGHT + SELECTION_SIGN_OFFSET}, 1, COLOR_WHITE);
		}

		Point effectAreaPoint = {0, START_HEIGHT};
		Point lineStartPoint = {0, effectAreaPoint.Y};

		// Input Sign
		{
			const uint8 INPUT_SIGN_LINES_OFFSET = 5;

			Point to = {effectAreaPoint.X + INPUT_SIGN_LINES_OFFSET, lineStartPoint.Y};
			Canvas.DrawLine({effectAreaPoint.X, to.Y - INPUT_SIGN_LINES_OFFSET}, to, LINE_COLOR);
			Canvas.DrawLine({effectAreaPoint.X, to.Y + INPUT_SIGN_LINES_OFFSET}, to, LINE_COLOR);

			to.X += INPUT_SIGN_LINES_OFFSET;
			Canvas.DrawLine({effectAreaPoint.X + INPUT_SIGN_LINES_OFFSET, to.Y - INPUT_SIGN_LINES_OFFSET}, to, LINE_COLOR);
			Canvas.DrawLine({effectAreaPoint.X + INPUT_SIGN_LINES_OFFSET, to.Y + INPUT_SIGN_LINES_OFFSET}, to, LINE_COLOR);

			Canvas.DrawLine(to, {canvasDimensions.X - lineStartPoint.X, lineStartPoint.Y}, LINE_COLOR);
		}

		uint8 effectCountInCurrentLine = 0;

		Point effectPoint = {effectAreaPoint.X, lineStartPoint.Y - HALF_EFFECT_DIMENSIONS.Y};

		effectPoint.X += EFFECTS_OFFSET;

		Effect **effects = preset->GetEffects();
		for (uint8 i = 0; i < Preset::EFFECT_COUNT; ++i)
		{
			Effect *effect = effects[i];

			if (m_SelectionType == SelectionTypes::Effect && i == m_SelectedEffectIndex)
				DrawSelectionSign(Canvas, {effectPoint.X + HALF_EFFECT_DIMENSIONS.X, effectPoint.Y - SELECTION_SIGN_OFFSET}, -1, COLOR_WHITE);

			Canvas.DrawFilledRectangle({effectPoint, EFFECT_DIMENSIONS}, (effect->GetIsEnabled() ? ENABLED_EFFECT_COLOR : DISABLED_EFFECT_COLOR));
			Canvas.DrawFilledCircle({effectPoint.X + HALF_EFFECT_DIMENSIONS.X, effectPoint.Y + (EFFECT_DIMENSIONS.Y / 3)}, EFFECT_DIMENSIONS.X / 5, BACKGROUND_FILL_COLOR);
			DrawStringJustified(Canvas, {effectPoint.X - HALF_EFFECT_DIMENSIONS.X, effectPoint.Y + EFFECT_DIMENSIONS.Y + 5, EFFECT_DIMENSIONS.X * 2, 20}, effect->GetName(), EFFECT_NAME_FONT, EFFECT_NAME_COLOR);

			effectPoint.X += EFFECTS_OFFSET;

			if (++effectCountInCurrentLine == MAX_EFFECT_PER_LINE)
			{
				lineStartPoint.X = 0;
				lineStartPoint.Y += LINES_OFFSET;

				effectPoint = {effectAreaPoint.X + HALF_EFFECTS_OFFSET, lineStartPoint.Y - HALF_EFFECT_DIMENSIONS.Y};

				Canvas.DrawLine(lineStartPoint, {canvasDimensions.X - lineStartPoint.X, lineStartPoint.Y}, LINE_COLOR);
			}
		}
	}

	void Activate(void) override
	{
		Screen::Activate();

		m_SelectionType = SelectionTypes::Name;
		m_SelectedEffectIndex = 0;

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<PresetScreen *>(Context);

													 thisPtr->value += Direction;
													 Log::WriteInfo("%i", thisPtr->value);

													 if (thisPtr->m_SelectionType == SelectionTypes::Effect)
													 {
														 int8 newIndex = thisPtr->m_SelectedEffectIndex + Direction;

														 if (newIndex < 0)
														 {
															 thisPtr->m_SelectionType = SelectionTypes::Volume;
														 }
														 else if (newIndex < 0 || Preset::EFFECT_COUNT <= newIndex)
														 {
															 thisPtr->m_SelectionType = SelectionTypes::Name;
														 }
														 else
															 thisPtr->m_SelectedEffectIndex = newIndex;
													 }
													 else if (thisPtr->m_SelectionType == SelectionTypes::Name)
													 {
														 if (Direction > 1)
															 thisPtr->m_SelectionType = SelectionTypes::Volume;
														 else
														 {
															 thisPtr->m_SelectionType = SelectionTypes::Effect;
															 thisPtr->m_SelectedEffectIndex = Preset::EFFECT_COUNT - 1;
														 }
													 }
													 else if (thisPtr->m_SelectionType == SelectionTypes::Volume)
													 {
														 if (Direction > 1)
														 {
															 thisPtr->m_SelectionType = SelectionTypes::Effect;
															 thisPtr->m_SelectedEffectIndex = 0;
														 }
														 else
															 thisPtr->m_SelectionType = SelectionTypes::Name;
													 }

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonCallback({this,
												[](void *Context, float HeldTime)
												{
													auto *thisPtr = static_cast<PlayScreen *>(Context);
												}});
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		auto *controlManager = GetControlManager();

		controlManager->SetUpButtonCallback(nullptr);
		controlManager->SetDownButtonCallback(nullptr);
		controlManager->SetValueButtonCallback(nullptr);
	}

	static void DrawSelectionSign(LCDCanvas &Canvas, Point Position, int8 UpDirection, Color Color)
	{
		const int8 POSITIVE_OFFSET_FROM_CENTER = UpDirection * 4;

		Point tipPoint = {Position.X, Position.Y + -POSITIVE_OFFSET_FROM_CENTER};
		Point rightPoint = {Position.X - POSITIVE_OFFSET_FROM_CENTER, Position.Y + POSITIVE_OFFSET_FROM_CENTER};
		Point leftPoint = {Position.X + POSITIVE_OFFSET_FROM_CENTER, Position.Y + POSITIVE_OFFSET_FROM_CENTER};

		Canvas.DrawFilledTriangle(leftPoint, tipPoint, rightPoint, Color);
	}

private:
	SelectionTypes m_SelectionType;
	uint8 m_SelectedEffectIndex;
	int32 value;
};

#endif