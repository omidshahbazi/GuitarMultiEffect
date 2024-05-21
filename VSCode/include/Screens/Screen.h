#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include "../framework/LCDCanvas.h"

const Color COLOR_BLACK = {0, 0, 0};
const Color COLOR_PURPLE = {180, 180, 255};
const Color COLOR_YELLOW = {255, 245, 130};

const Font &FONT_20 = Font_DUBAI_MEDIUM_20;
const Font &FONT_32 = Font_DUBAI_MEDIUM_32;

const Color PRESET_VOLUME_BOX_COLOR = COLOR_PURPLE;
const Font &PRESET_VOLUME_TEXT_FONT = FONT_20;

const Color PRESET_INFO_BOX_COLOR = COLOR_YELLOW;
const Color PRESET_INFO_TEXT_COLOR = COLOR_BLACK;
const Font &PRESET_INFO_TEXT_FONT = FONT_32;

class Screen
{
public:
	virtual void Draw(LCDCanvas &Canvas) = 0;
};

#endif