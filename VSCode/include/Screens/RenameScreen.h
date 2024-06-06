#pragma once
#ifndef RENAME_SCREEN_H
#define RENAME_SCREEN_H

#include "Screen.h"

class RenameScreen : public Screen
{
public:
	RenameScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_NameCharacterIndex(0),
		  m_NameCharacterIsSelected(false),
		  m_CharacterIndex(0)
	{
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		const Font &CHARACTER_FONT = FONT_24;
		const uint8 CHARACTERS_LINE_COUNT = 2;
		const uint8 CHARACTERS_PER_LINE = GetCharactersLength() / CHARACTERS_LINE_COUNT;
		const Point PER_LINE_CHARACTERS_DIMENSIONS = Canvas.MeasureStringDimension("", CHARACTERS_PER_LINE, CHARACTER_FONT);
		const uint8 CHARACTER_FONT_CHARACTER_WIDTH = PER_LINE_CHARACTERS_DIMENSIONS.X / CHARACTERS_PER_LINE;
		const uint8 CHARACTER_FONT_CHARACTER_HEIGHT = PER_LINE_CHARACTERS_DIMENSIONS.Y;
		const uint8 CHARACTER_HOVER_OFFSET = 3;
		const Font &NAME_FONT = FONT_32;
		const Point MAX_NAME_DIMENSIONS = Canvas.MeasureStringDimension("", Preset::MAX_NAME_LENGTH, NAME_FONT);
		const uint8 NAME_FONT_CHARACTER_WIDTH = MAX_NAME_DIMENSIONS.X / Preset::MAX_NAME_LENGTH;
		const uint8 NAME_FONT_CHARACTER_HEIGHT = MAX_NAME_DIMENSIONS.Y;
		const uint8 NAME_HOVER_OFFSET = 5;

		Screen::Draw(Canvas);

		DrawHeader(Canvas, DEFAULT_HEADER_HEIGHT,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, GetPresetNumber().c_str(), HEADER_DEFAULT_LEFT_TEXT_FONT, HEADER_DEFAULT_LEFT_TEXT_COLOR,
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "RENAME", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		auto &canvasDimensions = Canvas.GetDimension();

		Rect rect;
		for (uint8 i = 0; i < CHARACTERS_LINE_COUNT; ++i)
		{
			rect = {{0, DEFAULT_HEADER_HEIGHT + 50 + (i * CHARACTER_FONT_CHARACTER_HEIGHT)}, {canvasDimensions.X, CHARACTER_FONT_CHARACTER_HEIGHT}};

			Rect nameRect = rect;
			nameRect.Position.X = (canvasDimensions.X * 0.5) - (PER_LINE_CHARACTERS_DIMENSIONS.X * 0.5);

			if (m_NameCharacterIsSelected)
				if (i * CHARACTERS_PER_LINE <= m_CharacterIndex && m_CharacterIndex < (i + 1) * CHARACTERS_PER_LINE)
				{
					uint8 rowCharIndex = m_CharacterIndex % CHARACTERS_PER_LINE;

					Canvas.DrawFilledRectangle({{nameRect.Position.X + (rowCharIndex * CHARACTER_FONT_CHARACTER_WIDTH), nameRect.Position.Y + NAME_HOVER_OFFSET}, {CHARACTER_FONT_CHARACTER_WIDTH - NAME_HOVER_OFFSET, CHARACTER_FONT_CHARACTER_HEIGHT - CHARACTER_HOVER_OFFSET}}, COLOR_GREEN);
				}
			Canvas.DrawString(nameRect.Position, GetCharacters() + (i * CHARACTERS_PER_LINE), CHARACTERS_PER_LINE, CHARACTER_FONT, COLOR_WHITE);
		}

		auto &presetData = GetPresetManager()->GetSelectedPreset()->GetData();

		Rect nameRect = rect;
		nameRect.Position.X = (canvasDimensions.X * 0.5) - (MAX_NAME_DIMENSIONS.X * 0.5);
		nameRect.Position.Y += 50;
		nameRect.Dimension.X = MAX_NAME_DIMENSIONS.X;
		nameRect.Dimension.Y = NAME_FONT_CHARACTER_HEIGHT;

		Canvas.DrawFilledRectangle({{nameRect.Position.X + (m_NameCharacterIndex * NAME_FONT_CHARACTER_WIDTH), nameRect.Position.Y + NAME_HOVER_OFFSET}, {NAME_FONT_CHARACTER_WIDTH - NAME_HOVER_OFFSET, NAME_FONT_CHARACTER_HEIGHT - NAME_HOVER_OFFSET}}, COLOR_GREEN);
		Canvas.DrawString(nameRect.Position, presetData.Name, NAME_FONT, COLOR_WHITE);
	}

	void Activate(void) override
	{
		Screen::Activate();

		auto *controlManager = GetControlManager();

		m_NameCharacterIndex = 0;
		m_NameCharacterIsSelected = false;
		m_CharacterIndex = 0;

		controlManager->SetValueRotatedCallback({this,
												 [](void *Context, int8 Direction)
												 {
													 auto thisPtr = static_cast<RenameScreen *>(Context);

													 if (thisPtr->m_NameCharacterIsSelected)
													 {
														 thisPtr->m_CharacterIndex = Math::Wrap(thisPtr->m_CharacterIndex + Direction, 0, 39);

														 auto &presetData = thisPtr->GetPresetManager()->GetSelectedPreset()->GetData();
														 presetData.Name[thisPtr->m_NameCharacterIndex] = GetCharacters()[thisPtr->m_CharacterIndex];
													 }
													 else
														 thisPtr->m_NameCharacterIndex = Math::Wrap(thisPtr->m_NameCharacterIndex + Direction, 0, Preset::MAX_NAME_LENGTH - 1);

													 thisPtr->MarkAsDirty();
												 }});

		controlManager->SetValueButtonTunedOffCallback({this,
														[](void *Context, float HeldTime)
														{
															auto *thisPtr = static_cast<RenameScreen *>(Context);

															thisPtr->m_NameCharacterIsSelected = !thisPtr->m_NameCharacterIsSelected;

															if (thisPtr->m_NameCharacterIsSelected)
															{
																auto &presetData = thisPtr->GetPresetManager()->GetSelectedPreset()->GetData();
																char ch = presetData.Name[thisPtr->m_NameCharacterIndex];

																if ('A' <= ch && ch <= 'Z')
																	thisPtr->m_CharacterIndex = ch - 'A';
																else if ('0' <= ch && ch <= '9')
																	thisPtr->m_CharacterIndex = (ch - '0') + ('Z' - 'A');
																else if (ch == '-')
																	thisPtr->m_CharacterIndex = ('Z' - 'A') + ('9' - '0');
																else if (ch == '_')
																	thisPtr->m_CharacterIndex = ('Z' - 'A') + ('9' - '0') + 1;
																else if (ch == '/')
																	thisPtr->m_CharacterIndex = ('Z' - 'A') + ('9' - '0') + 2;
																else if (ch == ' ')
																	thisPtr->m_CharacterIndex = ('Z' - 'A') + ('9' - '0') + 3;
															}

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
	static cstr GetCharacters(void)
	{
		static const char CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_/ ";

		return CHARACTERS;
	}

	static uint8 GetCharactersLength(void)
	{
		return 40;
	}

private:
	uint8 m_NameCharacterIndex;
	bool m_NameCharacterIsSelected;
	uint8 m_CharacterIndex;
};

#endif