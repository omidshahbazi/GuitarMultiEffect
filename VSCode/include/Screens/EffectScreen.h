#pragma once
#ifndef EFFECT_SCREEN_H
#define EFFECT_SCREEN_H

#include "Screen.h"
#include "PresetScreen.h"
#include "../Effects/ReverbEffect.h"

// #pragma GCC pop_options

class EffectScreen : public Screen
{
private:
	struct PotentiometerData
	{
	public:
		float *Value;
		float MinValue;
		float MaxValue;
		float DisplayMultiplier;
		bool DisplayAsFloat;
		cstr Title;
	};

	static constexpr uint8 MAX_POTENTIOMENTER_COUNT = 6;

public:
	EffectScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_PotentiometerData{},
		  m_PotentiometerCount(0),
		  m_SelectedPotentiometerIndex(0),
		  m_IsSelected(false)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		Screen::Draw(Canvas);

		const Point &canvasDimensions = Canvas.GetDimension();

		const Color NORMAL_POTENTIOMETER_COLOR = COLOR_WHITE;
		const Color HOVERED_POTENTIOMETER_COLOR = COLOR_LIGHT_BLUE;
		const Color SELECTED_POTENTIOMETER_COLOR = COLOR_YELLOW;

		const uint8 HEADER_HEIGTH = 30;
		const Point POTENTIOMETER_DIMENSIONS = {90, 90};
		const uint8 MAX_LINE_COUNT = 2;
		const uint8 MAX_PER_LINE = MAX_POTENTIOMENTER_COUNT / MAX_LINE_COUNT;
		const uint8 PER_LINE_COUNT = Math::Min(MAX_PER_LINE, m_PotentiometerCount);
		const uint8 LINE_COUNT = Math::Ceil((float)m_PotentiometerCount / MAX_PER_LINE);
		const uint8 HORIZONTAL_OFFSET = canvasDimensions.X / PER_LINE_COUNT;
		const uint8 VERTICAL_OFFSET = (canvasDimensions.Y - HEADER_HEIGTH) / Math::Min(MAX_LINE_COUNT, LINE_COUNT);

		auto preset = GetPresetManager()->GetSelectedPreset();
		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, preset->GetEffects()[g_SelectedEffectIndex]->GetName(), HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		Rect potRect = {{}, POTENTIOMETER_DIMENSIONS};
		for (uint8 i = 0; i < m_PotentiometerCount; ++i)
		{
			const auto &data = m_PotentiometerData[i];

			potRect.Position.X = (HORIZONTAL_OFFSET * (i % PER_LINE_COUNT)) + ((HORIZONTAL_OFFSET * 0.5) - (POTENTIOMETER_DIMENSIONS.X * 0.5));
			potRect.Position.Y = HEADER_HEIGTH + (VERTICAL_OFFSET * (i % LINE_COUNT)) + ((VERTICAL_OFFSET * 0.5) - (POTENTIOMETER_DIMENSIONS.Y * 0.5));

			Color color = NORMAL_POTENTIOMETER_COLOR;
			if (i == m_SelectedPotentiometerIndex)
				if (m_IsSelected)
					color = SELECTED_POTENTIOMETER_COLOR;
				else
					color = HOVERED_POTENTIOMETER_COLOR;

			DrawPotentiometer(Canvas, potRect, data, FONT_16, color);

			potRect.Position.X += potRect.Dimension.X;
		}
	}

	void Activate(void) override
	{
		Screen::Activate();

		static const float VALUE_SPEEED = 0.01;

		m_PotentiometerCount = 0;
		m_SelectedPotentiometerIndex = 0;
		m_IsSelected = false;

		auto &presetData = GetPresetManager()->GetSelectedPreset()->GetData();
		auto effectData = presetData.EffectsData[g_SelectedEffectIndex];

		if (effectData->Index == presetData.OverdriveData.Index)
		{
			auto &effect = presetData.OverdriveData;

			PotentiometerData &rateData = m_PotentiometerData[m_PotentiometerCount++];
			rateData.Value = &effect.Rate;
			rateData.MinValue = 0;
			rateData.MaxValue = 1;
			rateData.DisplayMultiplier = 100;
			rateData.DisplayAsFloat = false;
			rateData.Title = "Rate";

			PotentiometerData &gainData = m_PotentiometerData[m_PotentiometerCount++];
			gainData.Value = &effect.Gain;
			gainData.MinValue = 0;
			gainData.MaxValue = 1;
			gainData.DisplayMultiplier = 100;
			rateData.DisplayAsFloat = false;
			gainData.Title = "Gain";
		}
		else if (effectData->Index == presetData.ReverbData.Index)
		{
			auto &effect = presetData.ReverbData;

			PotentiometerData &delayTimeData = m_PotentiometerData[m_PotentiometerCount++];
			delayTimeData.Value = &effect.DelayTime;
			delayTimeData.MinValue = 0;
			delayTimeData.MaxValue = ReverbEffect::MAX_DELAY_TIME;
			delayTimeData.DisplayMultiplier = 1;
			delayTimeData.DisplayAsFloat = true;
			delayTimeData.Title = "Length";

			PotentiometerData &feedbackData = m_PotentiometerData[m_PotentiometerCount++];
			feedbackData.Value = &effect.Feedback;
			feedbackData.MinValue = 0;
			feedbackData.MaxValue = 1;
			feedbackData.DisplayMultiplier = 100;
			feedbackData.DisplayAsFloat = false;
			feedbackData.Title = "Feedback";

			PotentiometerData &wetData = m_PotentiometerData[m_PotentiometerCount++];
			wetData.Value = &effect.WetRate;
			wetData.MinValue = 0;
			wetData.MaxValue = 1;
			wetData.DisplayMultiplier = 100;
			wetData.DisplayAsFloat = false;
			wetData.Title = "Wet";
		}

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<EffectScreen *>(Context);

													 if (thisPtr->m_IsSelected)
													 {
														 PotentiometerData &data = thisPtr->m_PotentiometerData[thisPtr->m_SelectedPotentiometerIndex];

														 auto &value = *data.Value;
														 value = Math::Clamp(value + (Direction * VALUE_SPEEED), data.MinValue, data.MaxValue);

														 thisPtr->GetPresetManager()->GetSelectedPreset()->UpdateData();
													 }
													 else
														 thisPtr->m_SelectedPotentiometerIndex = Math::Wrap(thisPtr->m_SelectedPotentiometerIndex + Direction, 0, thisPtr->m_PotentiometerCount - 1);

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonTunedOffCallback({this,
														[](void *Context, float HeldTime)
														{
															auto *thisPtr = static_cast<EffectScreen *>(Context);

															thisPtr->m_IsSelected = !thisPtr->m_IsSelected;

															thisPtr->MarkAsDirty();
														}});
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback(nullptr);
		controlManager->SetValueButtonTunedOffCallback(nullptr);
	}

private:
	static void DrawPotentiometer(LCDCanvas &Canvas, Rect Rect, const PotentiometerData &Data, const Font &Font, Color Color)
	{
		const uint8 CIRCLE_RADIUS = Rect.Dimension.X * 0.5;
		const uint8 LINE_LENGTH = CIRCLE_RADIUS - 5;
		const float CUT_OFF_ANGEL = 45;
		const uint8 CUT_OFF_START = CIRCLE_RADIUS + (Math::Cos(CUT_OFF_ANGEL * Math::TO_RADIANS) * CIRCLE_RADIUS);
		const float ANGEL = (((1 - Math::Map(*Data.Value, Data.MinValue, Data.MaxValue, 0.0F, 1.0F)) * (360 - (CUT_OFF_ANGEL * 2))) - CUT_OFF_ANGEL) * Math::TO_RADIANS;

		Point circlePoint = Rect.Position;
		circlePoint.X += CIRCLE_RADIUS;
		circlePoint.Y += CIRCLE_RADIUS;
		Canvas.DrawCircle(circlePoint, CIRCLE_RADIUS, Color);

		::Rect cutOffRect = Rect;
		cutOffRect.Position.Y += CUT_OFF_START;
		cutOffRect.Dimension.Y = (CIRCLE_RADIUS * 2) - CUT_OFF_START;
		Canvas.DrawFilledRectangle(cutOffRect, BACKGROUND_FILL_COLOR);

		cutOffRect.Position.Y -= 16;

		std::string valueText;
		float value = *Data.Value * Data.DisplayMultiplier;
		if (Data.DisplayAsFloat)
			valueText = std::to_string(value);
		else
		{
			valueText = std::to_string((uint8)value);
			// valueText.erase(valueText.begin() + (valueText.length() - 4));
		}

		DrawStringJustified(Canvas, cutOffRect, valueText.c_str(), Font, Color);

		cutOffRect.Position.Y += 16;
		DrawStringJustified(Canvas, cutOffRect, Data.Title, Font, Color);

		Point linePoint = circlePoint;
		linePoint.X += Math::Cos(ANGEL) * LINE_LENGTH;
		linePoint.Y -= Math::Sin(ANGEL) * LINE_LENGTH;
		Canvas.DrawLine(circlePoint, linePoint, Color);
	}

private:
	PotentiometerData m_PotentiometerData[MAX_POTENTIOMENTER_COUNT];
	uint8 m_PotentiometerCount;
	uint8 m_SelectedPotentiometerIndex;
	bool m_IsSelected;
};

#endif