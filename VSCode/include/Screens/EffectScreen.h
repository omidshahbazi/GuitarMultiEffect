#pragma once
#ifndef EFFECT_SCREEN_H
#define EFFECT_SCREEN_H

#include "Screen.h"
#include "PresetScreen.h"

class EffectScreen : public Screen
{
private:
	struct ChoiceData
	{
	public:
		static constexpr uint8 MAX_TYPE_COUNT = 6;

		int32 *SelectedItem;
		cstr Items[MAX_TYPE_COUNT];
		uint8 ItemCount;
	};

	struct ValueData
	{
	public:
		float *Value;
		float MinValue;
		float MaxValue;
		float DisplayMultiplier;
		bool AsInteger;
		cstr Title;
	};

	static constexpr uint8 MAX_CHOICE_DATA_COUNT = 2;
	static constexpr uint8 MAX_VALUE_DATA_COUNT = 6;

public:
	EffectScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_ChoiceData{},
		  m_ChoiceDataCount(0),
		  m_ValueData{},
		  m_ValueDataCount(0),
		  m_SelectedDataIndex(0),
		  m_IsSelected(false)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		Screen::Draw(Canvas);

		auto &canvasDimensions = Canvas.GetDimension();

		const Color NORMAL_COLOR = COLOR_WHITE;
		const Color HOVERED_COLOR = COLOR_LIGHT_BLUE;
		const Color SELECTED_COLOR = COLOR_YELLOW;
		const uint8 HEADER_HEIGTH = 30;

		const Point REFERENCE_CHOICE_DIMENSIONS = {120, 50};
		const uint16 CHOICE_DIVISION_WIDTH = canvasDimensions.X / m_ChoiceDataCount;
		const uint16 CHOICE_WIDTH = Math::Min(REFERENCE_CHOICE_DIMENSIONS.X, CHOICE_DIVISION_WIDTH);
		const uint16 CHOICE_HEIGTH = (CHOICE_WIDTH / REFERENCE_CHOICE_DIMENSIONS.X) * REFERENCE_CHOICE_DIMENSIONS.Y;

		const Point REFERENCE_VALUE_DIMENSIONS = {90, 90};
		const uint16 VALUE_DIVISION_WIDTH = canvasDimensions.X / m_ValueDataCount;
		const uint16 VALUE_WIDTH = Math::Min(REFERENCE_VALUE_DIMENSIONS.X, VALUE_DIVISION_WIDTH);
		const uint16 VALUE_HEIGTH = (VALUE_WIDTH / REFERENCE_VALUE_DIMENSIONS.X) * REFERENCE_VALUE_DIMENSIONS.Y;

		auto preset = GetPresetManager()->GetSelectedPreset();
		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, preset->GetEffects()[g_SelectedEffectIndex]->GetName(), HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		Rect rect = {{0, HEADER_HEIGTH + 20}, {CHOICE_WIDTH, CHOICE_HEIGTH}};
		for (uint8 i = 0; i < m_ChoiceDataCount; ++i)
		{
			const auto &data = m_ChoiceData[i];

			rect.Position.X = (i * CHOICE_DIVISION_WIDTH) + ((CHOICE_DIVISION_WIDTH * 0.5) - (CHOICE_WIDTH * 0.5));

			Color color = NORMAL_COLOR;
			if (i == m_SelectedDataIndex)
				if (m_IsSelected)
					color = SELECTED_COLOR;
				else
					color = HOVERED_COLOR;

			DrawChoiceData(Canvas, rect, data, FONT_16, color);
		}

		rect = {{0, HEADER_HEIGTH + 100}, {VALUE_WIDTH, VALUE_HEIGTH}};
		for (uint8 i = 0; i < m_ValueDataCount; ++i)
		{
			const auto &data = m_ValueData[i];

			rect.Position.X = (i * VALUE_DIVISION_WIDTH) + ((VALUE_DIVISION_WIDTH * 0.5) - (VALUE_WIDTH * 0.5));

			Color color = NORMAL_COLOR;
			if (i + m_ChoiceDataCount == m_SelectedDataIndex)
				if (m_IsSelected)
					color = SELECTED_COLOR;
				else
					color = HOVERED_COLOR;

			DrawValueData(Canvas, rect, data, FONT_16, color);
		}
	}

	void Activate(void) override
	{
		Screen::Activate();

		static const float VALUE_SPEEED = 0.01;

		m_SelectedDataIndex = 0;
		m_IsSelected = false;

		RefreshData();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<EffectScreen *>(Context);

													 if (thisPtr->m_IsSelected)
													 {
														 if (thisPtr->m_SelectedDataIndex < thisPtr->m_ChoiceDataCount)
														 {
															 auto &data = thisPtr->m_ChoiceData[thisPtr->m_SelectedDataIndex];

															 auto &value = *data.SelectedItem;

															 value = Math::Wrap(value + Direction, 0, data.ItemCount - 1);

															 thisPtr->RefreshData();
														 }
														 else
														 {
															 auto &data = thisPtr->m_ValueData[thisPtr->m_SelectedDataIndex - thisPtr->m_ChoiceDataCount];

															 auto &value = *data.Value;
															 if (data.AsInteger)
																 value += Direction / data.DisplayMultiplier;
															 else
																 value += (Direction * VALUE_SPEEED);

															 value = Math::Clamp(value, data.MinValue, data.MaxValue);
														 }

														 thisPtr->GetPresetManager()->GetSelectedPreset()->UpdateData();
													 }
													 else
														 thisPtr->m_SelectedDataIndex = Math::Wrap(thisPtr->m_SelectedDataIndex + Direction, 0, (thisPtr->m_ChoiceDataCount + thisPtr->m_ValueDataCount) - 1);

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
	void RefreshData(void)
	{
		m_ChoiceDataCount = 0;
		m_ValueDataCount = 0;

		auto &presetData = GetPresetManager()->GetSelectedPreset()->GetData();
		auto effectData = presetData.EffectsData[g_SelectedEffectIndex];

#define ADD_CHOICE_DATA_2(effectName, parameterName, item1, item2)                                 \
	{                                                                                              \
		ASSERT(m_ChoiceDataCount < MAX_VALUE_DATA_COUNT, "Out of ChoiceData slots");               \
		auto &data = m_ChoiceData[m_ChoiceDataCount++];                                            \
		data.SelectedItem = reinterpret_cast<int32 *>(&presetData.effectName##Data.parameterName); \
		data.ItemCount = 0;                                                                        \
		data.Items[data.ItemCount++] = item1;                                                      \
		data.Items[data.ItemCount++] = item2;                                                      \
	}

#define ADD_VALUE_DATA(effectName, parameterName, minValue, maxValue, displayMultiplier, asInteger, title) \
	{                                                                                                      \
		ASSERT(m_ValueDataCount < MAX_VALUE_DATA_COUNT, "Out of ValueData slots");                         \
		auto &data = m_ValueData[m_ValueDataCount++];                                                      \
		data.Value = &presetData.effectName##Data.parameterName;                                           \
		data.MinValue = minValue;                                                                          \
		data.MaxValue = maxValue;                                                                          \
		data.DisplayMultiplier = displayMultiplier;                                                        \
		data.AsInteger = asInteger;                                                                        \
		data.Title = title;                                                                                \
	}
		if (effectData->Index == presetData.FXData.Index)
		{
			ADD_CHOICE_DATA_2(FX, Type, "AUTO WAH", "CRY BABY");

			if (presetData.FXData.Type == FXEffect::Data::Types::CryBaby)
				ADD_VALUE_DATA(FX, WahRatio, 0, 1, 100, true, "RATIO");
		}
		else if (effectData->Index == presetData.DsData.Index)
		{
			ADD_CHOICE_DATA_2(Ds, Type, "OVERDRIVE", "DISTORTION");

			if (presetData.DsData.Type == DsEffect::Data::Types::Overdrive)
			{
				ADD_VALUE_DATA(Ds, OverdriveDrive, 0, 1, 100, true, "DRIVE");
				ADD_VALUE_DATA(Ds, OverdriveGain, 0, 1, 100, true, "GAIN");
			}
			else if (presetData.DsData.Type == DsEffect::Data::Types::Distortion)
			{
				ADD_VALUE_DATA(Ds, DistortionRate, 0, 1, 100, true, "RATE");
				ADD_VALUE_DATA(Ds, DistortionGain, 0, 1, 100, true, "GAIN");
			}
		}
		else if (effectData->Index == presetData.EqData.Index)
		{
			ADD_VALUE_DATA(Eq, LowTone, -20, 20, 1, true, "LOW");
			ADD_VALUE_DATA(Eq, MidTone, -20, 20, 1, true, "MID");
			ADD_VALUE_DATA(Eq, HighTone, -20, 20, 1, true, "HIGH");
		}
		else if (effectData->Index == presetData.RevData.Index)
		{
			ADD_VALUE_DATA(Rev, DelayTime, 0, RevEffect::MAX_DELAY_TIME, 1, false, "LENGTH");
			ADD_VALUE_DATA(Rev, Feedback, 0, 1, 100, true, "FEEDBACK");
			ADD_VALUE_DATA(Rev, WetRate, 0, 1, 100, true, "WET");
		}
		else if (effectData->Index == presetData.ModData.Index)
		{
			ADD_VALUE_DATA(Mod, Depth, 0, ModEffect::Data::MAX_DEPTH, 1, true, "DEPTH");
			ADD_VALUE_DATA(Mod, Rate, 0.01, 4, 1, false, "RATE");
			ADD_VALUE_DATA(Mod, Feedback, 0, 1, 100, true, "FEEDBACK");
			ADD_VALUE_DATA(Mod, Wet, 0, 1, 100, true, "WET");
		}

#undef ADD_CHOICE_DATA_2
#undef ADD_VALUE_DATA
	}

private:
	static void
	DrawChoiceData(LCDCanvas &Canvas, Rect Rect, const ChoiceData &Data, const Font &Font, Color Color)
	{
		Canvas.DrawRectangle(Rect, Color);

		DrawStringJustified(Canvas, Rect, Data.Items[*Data.SelectedItem], Font, Color);
	}

	static void DrawValueData(LCDCanvas &Canvas, Rect Rect, const ValueData &Data, const Font &Font, Color Color)
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

		float value = *Data.Value * Data.DisplayMultiplier;

		std::string valueText;
		if (value < 0)
		{
			valueText = "-";
			value *= -1;
		}

		if (Data.AsInteger)
			valueText += std::to_string((uint32)value);
		else
		{
			valueText += std::to_string(value);
			valueText.erase(valueText.begin() + (valueText.length() - 4));
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
	ChoiceData m_ChoiceData[MAX_CHOICE_DATA_COUNT];
	uint8 m_ChoiceDataCount;
	ValueData m_ValueData[MAX_VALUE_DATA_COUNT];
	uint8 m_ValueDataCount;
	uint8 m_SelectedDataIndex;
	bool m_IsSelected;
};

#endif