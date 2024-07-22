#pragma once
#ifndef PRESET_SCREEN_H
#define PRESET_SCREEN_H

#include "Screen.h"

class PresetScreen : public Screen
{
public:
	PresetScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_SelectableItemCount(Preset::EFFECT_COUNT + 2),
		  m_IsEffectReordering(false),
		  m_ReorderingJustStarted(false)
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
		const Color SELECTED_EFFECT_COLOR = COLOR_RED;
		const Font &EFFECT_NAME_FONT = FONT_20;
		const Color EFFECT_NAME_COLOR = COLOR_LIGHT_BLUE;
		const uint16 START_HEIGHT = DEFAULT_HEADER_HEIGTH + 50;
		const uint16 LINES_OFFSET = 70;
		const Point EFFECT_DIMENSIONS = {20, 30};
		const Point HALF_EFFECT_DIMENSIONS = {EFFECT_DIMENSIONS.X * 0.5, EFFECT_DIMENSIONS.Y * 0.5};
		const uint8 LINE_COUNT = 2;
		const uint8 MAX_EFFECT_PER_LINE = Math::Ceil((Preset::EFFECT_COUNT + 1) / (float)LINE_COUNT);
		const uint8 EFFECTS_OFFSET = canvasDimensions.X / MAX_EFFECT_PER_LINE;
		const uint8 HALF_EFFECTS_OFFSET = EFFECTS_OFFSET * 0.5;
		const uint8 SELECTION_SIGN_OFFSET = 8;

		Screen::Draw(Canvas);

		Preset *preset = GetPresetManager()->GetSelectedPreset();
		uint8 selectedPointerIndex = preset->GetData().SelectedPointerIndex;

		auto &presetData = preset->GetData();
		DrawHeader(Canvas, DEFAULT_HEADER_HEIGTH,
				   PRESET_INDEX_BOX_COLOR, GetPresetNumber().c_str(), PRESET_INDEX_TEXT_FONT, PRESET_INDEX_TEXT_COLOR,
				   PRESET_NAME_BOX_COLOR, presetData.Name, PRESET_NAME_TEXT_FONT, PRESET_NAME_TEXT_COLOR,
				   PRESET_VOLUME_BOX_COLOR, GetPresetVolume().c_str(), PRESET_VOLUME_TEXT_FONT, PRESET_VOLUME_TEXT_COLOR);

		if (selectedPointerIndex == Preset::EFFECT_COUNT)
		{
			DrawSelectionSign(Canvas, {HEADER_LEFT_PART_WIDTH + (uint16)(HEADER_MIDDLE_PART_WIDTH * 0.5), DEFAULT_HEADER_HEIGTH + SELECTION_SIGN_OFFSET}, 1, COLOR_WHITE);
		}
		else if (selectedPointerIndex == Preset::EFFECT_COUNT + 1)
		{
			uint16 leftAndRightWidth = HEADER_LEFT_PART_WIDTH + HEADER_MIDDLE_PART_WIDTH;
			DrawSelectionSign(Canvas, {leftAndRightWidth + (uint16)((canvasDimensions.X - leftAndRightWidth) * 0.5), DEFAULT_HEADER_HEIGTH + SELECTION_SIGN_OFFSET}, 1, COLOR_WHITE);
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

		uint8 effectCountInCurrentLine = 1;

		Point effectPoint = {effectAreaPoint.X, lineStartPoint.Y - HALF_EFFECT_DIMENSIONS.Y};

		effectPoint.X += EFFECTS_OFFSET;

		Effect **effects = preset->GetEffects();
		for (uint8 i = 0; i < Preset::EFFECT_COUNT; ++i)
		{
			Effect *effect = effects[i];

			if (i == selectedPointerIndex)
				DrawSelectionSign(Canvas, {effectPoint.X + HALF_EFFECT_DIMENSIONS.X, effectPoint.Y - SELECTION_SIGN_OFFSET}, -1, COLOR_WHITE);

			// Pedal Shape
			{
				Color color = ENABLED_EFFECT_COLOR;
				if (m_IsEffectReordering && i == selectedPointerIndex)
					color = SELECTED_EFFECT_COLOR;
				else if (!effect->GetIsEnabled())
					color = DISABLED_EFFECT_COLOR;

				Canvas.DrawFilledRectangle({effectPoint, EFFECT_DIMENSIONS}, color);
				Canvas.DrawFilledCircle({effectPoint.X + HALF_EFFECT_DIMENSIONS.X, effectPoint.Y + (EFFECT_DIMENSIONS.Y / 4)}, EFFECT_DIMENSIONS.X / 6, BACKGROUND_FILL_COLOR);
				Canvas.DrawFilledCircle({effectPoint.X + HALF_EFFECT_DIMENSIONS.X, effectPoint.Y + (3 * EFFECT_DIMENSIONS.Y / 4)}, EFFECT_DIMENSIONS.X / 10, BACKGROUND_FILL_COLOR);
			}

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
#define GET_PRESET_DATA() thisPtr->GetPresetManager()->GetSelectedPreset()->GetData()
#define UPDATE_PRESET() thisPtr->GetPresetManager()->GetSelectedPreset()->UpdateData()
#define GET_EFFECT_DATA(Index) GET_PRESET_DATA().EffectsData[Index]
#define UPDATE_ENABLED_LED(Index) thisPtr->GetControlManager()->SetVariation3LEDConstantBrightness(GET_EFFECT_DATA(Index)->Enabled ? COLOR_GREEN : COLOR_BLACK)

		Screen::Activate();

		auto *controlManager = GetControlManager();

		m_IsEffectReordering = false;
		m_ReorderingJustStarted = false;

		controlManager->SetVariation3LEDConstantBrightness(COLOR_BLACK);

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<PresetScreen *>(Context);

													 if (thisPtr->m_IsEffectReordering)
													 {
														 uint8 oldIndex = GET_PRESET_DATA().SelectedPointerIndex;

														 GET_PRESET_DATA().SelectedPointerIndex = Math::Clamp(GET_PRESET_DATA().SelectedPointerIndex + Direction, 0, Preset::EFFECT_COUNT - 1);

														 GET_EFFECT_DATA(GET_PRESET_DATA().SelectedPointerIndex)->Index = oldIndex;
														 GET_EFFECT_DATA(oldIndex)->Index = GET_PRESET_DATA().SelectedPointerIndex;

														 UPDATE_PRESET();
													 }
													 else
													 {
														 GET_PRESET_DATA().SelectedPointerIndex = Math::Wrap(GET_PRESET_DATA().SelectedPointerIndex + Direction, 0, (int32)thisPtr->m_SelectableItemCount - 1);

														 if (GET_PRESET_DATA().SelectedPointerIndex >= Preset::EFFECT_COUNT)
															 thisPtr->GetControlManager()->SetVariation3LEDConstantBrightness(COLOR_BLACK);
														 else
															 UPDATE_ENABLED_LED(GET_PRESET_DATA().SelectedPointerIndex);
													 }

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonHoldCallback({this,
													[](void *Context, float HeldTime)
													{
														if (HeldTime < 2)
															return;

														auto *thisPtr = static_cast<PresetScreen *>(Context);

														if (thisPtr->m_IsEffectReordering)
															return;

														if (GET_PRESET_DATA().SelectedPointerIndex >= Preset::EFFECT_COUNT)
															return;

														thisPtr->m_ReorderingJustStarted = true;
														thisPtr->m_IsEffectReordering = true;

														thisPtr->MarkAsDirty();
													}});

		controlManager->SetValueButtonTurnedOffCallback({this,
														 [](void *Context, float HeldTime)
														 {
															 auto *thisPtr = static_cast<PresetScreen *>(Context);

															 if (thisPtr->m_ReorderingJustStarted)
															 {
																 thisPtr->m_ReorderingJustStarted = false;
																 return;
															 }
															 else if (thisPtr->m_IsEffectReordering)
																 thisPtr->m_IsEffectReordering = false;
															 else if (GET_PRESET_DATA().SelectedPointerIndex < Preset::EFFECT_COUNT)
																 thisPtr->SwitchScreen(Screens::Effect);
															 else if (GET_PRESET_DATA().SelectedPointerIndex == Preset::EFFECT_COUNT)
																 thisPtr->SwitchScreen(Screens::Rename);
															 else if (GET_PRESET_DATA().SelectedPointerIndex == Preset::EFFECT_COUNT + 1)
																 thisPtr->SwitchScreen(Screens::Level);

															 thisPtr->MarkAsDirty();
														 }});

		controlManager->SetVariation3ButtonTurnedOffCallback({this,
															  [](void *Context, float HeldTime)
															  {
																  auto *thisPtr = static_cast<PresetScreen *>(Context);

																  if (GET_PRESET_DATA().SelectedPointerIndex >= Preset::EFFECT_COUNT)
																	  return;

																  auto *effectData = GET_EFFECT_DATA(GET_PRESET_DATA().SelectedPointerIndex);

																  effectData->Enabled = !effectData->Enabled;

																  UPDATE_ENABLED_LED(GET_PRESET_DATA().SelectedPointerIndex);

																  UPDATE_PRESET();

																  thisPtr->MarkAsDirty();
															  }});

#undef GET_PRESET_DATA
#undef UPDATE_PRESET
#undef GET_EFFECT_DATA
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback(nullptr);
		controlManager->SetValueButtonHoldCallback(nullptr);
		controlManager->SetValueButtonTurnedOffCallback(nullptr);
		controlManager->SetVariation3LEDConstantBrightness(COLOR_BLACK);
		controlManager->SetVariation3ButtonTurnedOffCallback(nullptr);
	}

private:
	static void DrawSelectionSign(LCDCanvas &Canvas, Point Position, int8 UpDirection, Color Color)
	{
		const int8 POSITIVE_OFFSET_FROM_CENTER = UpDirection * 4;

		Point tipPoint = {Position.X, Position.Y + -POSITIVE_OFFSET_FROM_CENTER};
		Point rightPoint = {Position.X - POSITIVE_OFFSET_FROM_CENTER, Position.Y + POSITIVE_OFFSET_FROM_CENTER};
		Point leftPoint = {Position.X + POSITIVE_OFFSET_FROM_CENTER, Position.Y + POSITIVE_OFFSET_FROM_CENTER};

		Canvas.DrawFilledTriangle(leftPoint, tipPoint, rightPoint, Color);
	}

private:
	uint8 m_SelectableItemCount;
	bool m_IsEffectReordering;
	bool m_ReorderingJustStarted;
};

#endif