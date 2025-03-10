#pragma once
#ifndef PARAMETER_SCREEN_H
#define PARAMETER_SCREEN_H

#include "Screen.h"

class ParameterScreen : public Screen
{
protected:
	struct ChoiceData
	{
	public:
		static constexpr uint8 MAX_ITEM_COUNT = 12;

		int32 *SelectedItem;
		cstr Items[MAX_ITEM_COUNT];
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
		float Speed;
		cstr Title;
	};

private:
	static constexpr uint8 MAX_CHOICE_DATA_COUNT = 2;
	static constexpr uint8 MAX_VALUE_DATA_COUNT = 8;

public:
	ParameterScreen(PresetManager *PresetManager, ControlManager *ControlManager)
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

		const Point REFERENCE_CHOICE_DIMENSIONS = {120, 40};
		const uint16 CHOICE_DIVISION_WIDTH = canvasDimensions.X / m_ChoiceDataCount;
		const uint16 CHOICE_WIDTH = Math::Min(REFERENCE_CHOICE_DIMENSIONS.X, CHOICE_DIVISION_WIDTH);
		const uint16 CHOICE_HEIGTH = (CHOICE_WIDTH / REFERENCE_CHOICE_DIMENSIONS.X) * REFERENCE_CHOICE_DIMENSIONS.Y;

		const Point VALUE_DIMENSIONS = {60, 60};
		const uint8 MAX_VALUE_PER_LINE = 4;
		const uint8 VALUE_LINE_COUNT = Math::Ceil(m_ValueDataCount / (float)MAX_VALUE_PER_LINE);

		DrawHeader(Canvas, HEADER_HEIGTH,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, GetTitle(), HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		Rect rect = {{0, HEADER_HEIGTH + 15}, {CHOICE_WIDTH, CHOICE_HEIGTH}};
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

		const uint16 VALUE_AREA_START_Y = HEADER_HEIGTH + 50;
		const uint16 VALUE_AREA_HEIGTH = canvasDimensions.Y - VALUE_AREA_START_Y;
		rect = {{0, VALUE_AREA_START_Y + ((VALUE_AREA_HEIGTH / (VALUE_LINE_COUNT + 1)) - (VALUE_DIMENSIONS.Y * 0.5))}, VALUE_DIMENSIONS};
		for (uint8 l = 0; l < VALUE_LINE_COUNT; ++l)
		{
			const uint8 VALUE_COUNT = Math::Min(m_ValueDataCount - (l * MAX_VALUE_PER_LINE), MAX_VALUE_PER_LINE);
			const Point VALUE_SLOT_DIMENTIONS = {canvasDimensions.X / VALUE_COUNT, VALUE_DIMENSIONS.Y + 10};

			rect.Position.Y += l * VALUE_SLOT_DIMENTIONS.Y;

			for (uint8 i = 0; i < VALUE_COUNT; ++i)
			{
				uint8 itemIndex = (l * MAX_VALUE_PER_LINE) + i;
				const auto &data = m_ValueData[itemIndex];

				rect.Position.X = (i * VALUE_SLOT_DIMENTIONS.X) + ((VALUE_SLOT_DIMENTIONS.X * 0.5) - (VALUE_DIMENSIONS.X * 0.5));

				Color color = NORMAL_COLOR;
				if (itemIndex + m_ChoiceDataCount == m_SelectedDataIndex)
					if (m_IsSelected)
						color = SELECTED_COLOR;
					else
						color = HOVERED_COLOR;

				DrawValueData(Canvas, rect, data, FONT_16, color);
			}
		}
	}

	void Activate(void) override
	{
		Screen::Activate();

		m_SelectedDataIndex = 0;
		m_IsSelected = false;

		RefreshData();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<ParameterScreen *>(Context);

													 if (thisPtr->m_IsSelected)
													 {
														 if (thisPtr->m_SelectedDataIndex < thisPtr->m_ChoiceDataCount)
														 {
															 auto &data = thisPtr->m_ChoiceData[thisPtr->m_SelectedDataIndex];

															 auto &value = *data.SelectedItem;

															 value = Math::Wrap(value + Direction, 0, data.ItemCount - 1);

															 thisPtr->OnChoiceDataChanged();
														 }
														 else
														 {
															 auto &data = thisPtr->m_ValueData[thisPtr->m_SelectedDataIndex - thisPtr->m_ChoiceDataCount];

															 auto &value = *data.Value;
															 float direction = Direction * data.Speed;

															 if (data.AsInteger)
																 value += direction / data.DisplayMultiplier;
															 else
																 value += direction;

															 value = Math::Clamp(value, data.MinValue, data.MaxValue);

															 thisPtr->OnValueDataChanged();
														 }
													 }
													 else
														 thisPtr->m_SelectedDataIndex = Math::Wrap(thisPtr->m_SelectedDataIndex + Direction, 0, (thisPtr->m_ChoiceDataCount + thisPtr->m_ValueDataCount) - 1);

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonTurnedOffCallback({this,
														 [](void *Context, float HeldTime)
														 {
															 auto *thisPtr = static_cast<ParameterScreen *>(Context);

															 thisPtr->m_IsSelected = !thisPtr->m_IsSelected;

															 thisPtr->MarkAsDirty();
														 }});
	}

	void Deactivate(void) override
	{
		Screen::Deactivate();

		auto *controlManager = GetControlManager();

		controlManager->SetValueRotatedCallback(nullptr);
		controlManager->SetValueButtonTurnedOffCallback(nullptr);
	}

	void AddChoiceData(int32 *Data, cstr Item1, cstr Item2, cstr Item3 = nullptr, cstr Item4 = nullptr, cstr Item5 = nullptr, cstr Item6 = nullptr, cstr Item7 = nullptr, cstr Item8 = nullptr, cstr Item9 = nullptr, cstr Item10 = nullptr, cstr Item11 = nullptr, cstr Item12 = nullptr)
	{
		ASSERT(m_ChoiceDataCount < MAX_CHOICE_DATA_COUNT, "Out of ChoiceData slots");
		ASSERT(Data != nullptr, "Data cannot be null");
		ASSERT(Item1 != nullptr, "Item1 cannot be null");
		ASSERT(Item2 != nullptr, "Item2 cannot be null");

		auto &data = m_ChoiceData[m_ChoiceDataCount++];
		data.SelectedItem = Data;
		data.ItemCount = 0;
		data.Items[data.ItemCount++] = Item1;

		if (Item2 != nullptr)
			data.Items[data.ItemCount++] = Item2;
		if (Item3 != nullptr)
			data.Items[data.ItemCount++] = Item3;
		if (Item4 != nullptr)
			data.Items[data.ItemCount++] = Item4;
		if (Item5 != nullptr)
			data.Items[data.ItemCount++] = Item5;
		if (Item6 != nullptr)
			data.Items[data.ItemCount++] = Item6;
		if (Item7 != nullptr)
			data.Items[data.ItemCount++] = Item7;
		if (Item8 != nullptr)
			data.Items[data.ItemCount++] = Item8;
		if (Item9 != nullptr)
			data.Items[data.ItemCount++] = Item9;
		if (Item10 != nullptr)
			data.Items[data.ItemCount++] = Item10;
		if (Item11 != nullptr)
			data.Items[data.ItemCount++] = Item11;
		if (Item12 != nullptr)
			data.Items[data.ItemCount++] = Item12;
	}

	void AddValueData(float *Data, cstr Title, float MinValue, float MaxValue, float DisplayMultiplier, bool AsInteger, float Speed = 1)
	{
		ASSERT(m_ValueDataCount < MAX_VALUE_DATA_COUNT, "Out of ValueData slots");
		ASSERT(Data != nullptr, "Data cannot be null");
		ASSERT(Title != nullptr, "Title cannot be null");

		auto &data = m_ValueData[m_ValueDataCount++];
		data.Value = Data;
		data.MinValue = MinValue;
		data.MaxValue = MaxValue;
		data.DisplayMultiplier = DisplayMultiplier;
		data.AsInteger = AsInteger;
		data.Speed = Speed;
		data.Title = Title;
	}

	void RefreshData(void)
	{
		m_ChoiceDataCount = 0;
		m_ValueDataCount = 0;

		OnRefreshData();
	}

	virtual cstr GetTitle(void) const = 0;
	virtual void OnRefreshData(void) = 0;
	virtual void OnChoiceDataChanged(void)
	{
	}
	virtual void OnValueDataChanged(void)
	{
	}

private:
	static void DrawChoiceData(LCDCanvas &Canvas, Rect Rect, const ChoiceData &Data, const Font &Font, Color Color)
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