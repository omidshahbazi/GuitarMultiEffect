#pragma once
#ifndef LEVEL_SCREEN_H
#define LEVEL_SCREEN_H

#include "ParameterScreen.h"
#include "../framework/DSP/SampleAmountMeter.h"

class LevelScreen : public ParameterScreen
{
public:
	LevelScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: ParameterScreen(PresetManager, ControlManager)
	{
	}

	void ProcessAudioBuffer(const SampleType *const InputBuffer, const SampleType *const OutputBuffer, uint8 Count) override
	{
#define GAIN_INPUT_VALUE(Value) Math::Clamp01(Value *Math::dbToMultiplier(GetPresetManager()->GetSelectedPreset()->GetData().InputGain))

		for (uint8 i = 0; i < Count; ++i)
		{
			m_InputMeter.Record(InputBuffer[i]);
			m_OutputtMeter.Record(OutputBuffer[i]);
		}

		m_InputMaxLevel = Math::Max(0.0, m_InputMaxLevel - 0.0001);
		m_OutputMaxLevel = Math::Max(0.0, m_OutputMaxLevel - 0.0001);

		m_InputLevel = GAIN_INPUT_VALUE(m_InputMeter.GetAbsoluteMax());
		if (m_InputMaxLevel < m_InputLevel)
			m_InputMaxLevel = m_InputLevel;

		m_OutputLevel = m_OutputtMeter.GetAbsoluteMax();
		if (m_OutputMaxLevel < m_OutputLevel)
			m_OutputMaxLevel = m_OutputLevel;

		MarkAsDirty();

#undef GAIN_INPUT_VALUE
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		ParameterScreen::Draw(Canvas);

		auto &canvasDimension = Canvas.GetDimension();

		const uint16 LEVEL_BAR_WIDTH = canvasDimension.X - 50;

		Rect rect = {{(canvasDimension.X * 0.5) - (LEVEL_BAR_WIDTH * 0.5), DEFAULT_HEADER_HEIGHT + 10}, {LEVEL_BAR_WIDTH, 20}};

		DrawLevel(Canvas, rect, "IN", m_InputLevel, m_InputMaxLevel);

		rect.Position.Y += 30;
		DrawLevel(Canvas, rect, "OUT", m_OutputLevel, m_OutputMaxLevel);
	}

	void Activate(void) override
	{
		ParameterScreen::Activate();

		m_InputMeter.Reset();
		m_OutputtMeter.Reset();

		m_InputLevel = 0;
		m_InputMaxLevel = 0;

		m_OutputLevel = 0;
		m_OutputMaxLevel = 0;
	}

	cstr GetTitle(void) const override
	{
		return "LEVEL";
	}

	void OnValueDataChanged(void) override
	{
		GetPresetManager()->GetSelectedPreset()->UpdateData();
	}

	void OnRefreshData(void) override
	{
		auto &data = GetPresetManager()->GetSelectedPreset()->GetData();

		AddValueData(&data.InputGain, "IN GAIN", -20, 40, 1, true);
		AddValueData(&data.OutputGain, "OUT GAIN", -30, 40, 1, true);
		AddValueData(&data.OutputVolume, "VOL", 0, 1, 100, true);
	}

	static void DrawLevel(LCDCanvas &Canvas, Rect Rect, cstr Title, SampleType Level, SampleType MaxLevel)
	{
		::Rect valueRect = Rect;
		valueRect.Dimension.X *= Level;
		Canvas.DrawFilledRectangle(valueRect, COLOR_GREEN);

		DrawStringJustified(Canvas, Rect, Title, FONT_20, COLOR_WHITE);

		uint16 x = Rect.Position.X + (Rect.Dimension.X * MaxLevel);
		Canvas.DrawLine({x, Rect.Position.Y}, {x, Rect.Position.Y + Rect.Dimension.Y}, COLOR_RED);

		Canvas.DrawRectangle(Rect, COLOR_WHITE);
	}

private:
	// Bigger buffers would leads the code to a race condition between Audio thread and Main thread
	SampleAmountMeter<SampleType, 320> m_InputMeter;
	SampleAmountMeter<SampleType, 320> m_OutputtMeter;

	SampleType m_InputLevel;
	SampleType m_InputMaxLevel;

	SampleType m_OutputLevel;
	SampleType m_OutputMaxLevel;
};

#endif