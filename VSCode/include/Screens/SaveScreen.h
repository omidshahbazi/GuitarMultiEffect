#pragma once
#ifndef SAVE_SCREEN_H
#define SAVE_SCREEN_H

#include "Screen.h"

class SaveScreen : public Screen
{
public:
	SaveScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_SelectedIndex(0)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		auto &canvasDimension = Canvas.GetDimension();
		const uint8 HEADER_HEIGTH = 30;
		const uint8 HORIZONTAL_OFFSET = canvasDimension.X / 3;
		const uint8 VERTICAL_OFFSET = HEADER_HEIGTH + 100;
		const Point BUTTON_DIMENSIONS = {80, 40};

		Screen::Draw(Canvas);

		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "SAVE", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		Rect rect;
		rect.Position.X = HORIZONTAL_OFFSET - (BUTTON_DIMENSIONS.X * 0.5);
		rect.Position.Y = VERTICAL_OFFSET;
		rect.Dimension = BUTTON_DIMENSIONS;
		Canvas.DrawFilledRectangle(rect, COLOR_LIGHT_BLUE);
		if (m_SelectedIndex == 0)
			Canvas.DrawRectangle(rect, COLOR_YELLOW, 3);
		DrawStringJustified(Canvas, rect, "SAVE", FONT_24, COLOR_WHITE);

		rect.Position.X += HORIZONTAL_OFFSET;
		Canvas.DrawFilledRectangle(rect, COLOR_LIGHT_BLUE);
		if (m_SelectedIndex == 1)
			Canvas.DrawRectangle(rect, COLOR_YELLOW, 3);
		DrawStringJustified(Canvas, rect, "RESET", FONT_24, COLOR_WHITE);
	}

	void Activate(void) override
	{
		Screen::Activate();

		auto *controlManager = GetControlManager();

		m_SelectedIndex = 0;

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<SaveScreen *>(Context);

													 thisPtr->m_SelectedIndex = Math::Clamp01(thisPtr->m_SelectedIndex + Direction);

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonTurnedOffCallback({this,
														[](void *Context, float HeldTime)
														{
															auto *thisPtr = static_cast<SaveScreen *>(Context);

															if (thisPtr->m_SelectedIndex == 0)
															{
																thisPtr->GetPresetManager()->Save();
															}
															else if (thisPtr->m_SelectedIndex == 1)
															{
																thisPtr->GetPresetManager()->Ereas();
															}

															thisPtr->ClearScreensHistory();
															thisPtr->SwitchScreen(Screens::Play);
														}});
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback(nullptr);
		controlManager->SetValueButtonTurnedOffCallback(nullptr);
	}

private:
	uint8 m_SelectedIndex;
};

#endif