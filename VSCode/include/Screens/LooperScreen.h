#pragma once
#ifndef LOOPER_SCREEN_H
#define LOOPER_SCREEN_H

#include "Screen.h"

class LooperScreen : public Screen
{
public:
	LooperScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_IsRecording(0),
		  m_StartTime(0)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		auto &canvasDimension = Canvas.GetDimension();
		const uint16 LEVEL_BAR_WIDTH = 200;

		Screen::Draw(Canvas);

		DrawHeader(Canvas, DEFAULT_HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "LOOPER", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		uint32 elapsedTime = GetElapsedTime();
		float normalizedElapsedTime = GetNormalizedElapsedTime();

		Color progressColor = COLOR_GREEN;
		if (m_IsRecording)
			progressColor = COLOR_RED;

		Rect rect = {{(canvasDimension.X * 0.5) - (LEVEL_BAR_WIDTH * 0.5), DEFAULT_HEADER_HEIGTH + 10}, {LEVEL_BAR_WIDTH, 20}};
		DrawProgressBar(Canvas, rect, COLOR_WHITE, std::to_string(elapsedTime / 1000).c_str(), COLOR_WHITE, normalizedElapsedTime, progressColor, false, 0, COLOR_BLACK);

		MarkAsDirty();
	}

	void Activate(void) override
	{
		Screen::Activate();

		static auto *controlManager = GetControlManager();
		static auto *presetManager = GetPresetManager();
		static auto *hal = presetManager->GetHAL();

		controlManager->SetVariation3ButtonTurnedOnCallback({this,
															 [](void *Context)
															 {
																 auto *thisPtr = static_cast<LooperScreen *>(Context);

																 if (thisPtr->m_IsRecording)
																 {
																	 thisPtr->m_IsRecording = false;

																	 presetManager->GetLooper()->SetReplayMode(thisPtr->GetElapsedTime() / 1000.0);
																 }
																 else
																 {
																	 thisPtr->m_IsRecording = true;

																	 if (!thisPtr->m_HasRecordedAlready)
																	 {
																		 thisPtr->m_StartTime = hal->GetTimeSinceStartupMs();
																		 thisPtr->m_HasRecordedAlready = true;
																	 }

																	 presetManager->GetLooper()->SetRecordMode();
																 }
															 }});
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		static auto *controlManager = GetControlManager();

		controlManager->SetVariation3ButtonTurnedOnCallback(nullptr);
	}

	uint32 GetElapsedTime(void)
	{
		static auto presetManager = GetPresetManager();
		static auto looper = presetManager->GetLooper();
		const uint32 MAX_TIME = looper->GetMaxTime() * 1000;
		uint16 currTime = presetManager->GetHAL()->GetTimeSinceStartupMs();

		return Math::Wrap(currTime - m_StartTime, 0, MAX_TIME);
	}

	float GetNormalizedElapsedTime(void)
	{
		static auto presetManager = GetPresetManager();
		static auto looper = presetManager->GetLooper();
		const uint32 MAX_TIME = looper->GetMaxTime() * 1000;

		return GetElapsedTime() / (float)MAX_TIME;
	}

private:
	bool m_HasRecordedAlready;
	bool m_IsRecording;
	uint32 m_StartTime;
};

#endif