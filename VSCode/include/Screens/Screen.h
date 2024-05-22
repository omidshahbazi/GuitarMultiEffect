#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include "../Presets/PresetManager.h"
#include "../framework/LCDCanvas.h"

const Color COLOR_BLACK = {0, 0, 0};
const Color COLOR_PURPLE = {180, 180, 255};
const Color COLOR_YELLOW = {255, 245, 130};

const Font &FONT_20 = Font_DUBAI_MEDIUM_20;
#ifdef DEBUG
const Font &FONT_32 = Font_DUBAI_MEDIUM_20;
#else
const Font &FONT_32 = Font_DUBAI_MEDIUM_32;
#endif

const Color PRESET_VOLUME_BOX_COLOR = COLOR_PURPLE;
const Font &PRESET_VOLUME_TEXT_FONT = FONT_20;

const Color PRESET_INFO_BOX_COLOR = COLOR_YELLOW;
const Color PRESET_INFO_TEXT_COLOR = COLOR_BLACK;
const Font &PRESET_INFO_TEXT_FONT = FONT_32;

class Screen
{
public:
	Screen(PresetManager *PresetManager)
		: m_PresetManager(PresetManager)
	{
	}

	virtual void Draw(LCDCanvas &Canvas) = 0;

	PresetManager *GetPresetManager(void)
	{
		return m_PresetManager;
	}

private:
	PresetManager *m_PresetManager;
};

#endif