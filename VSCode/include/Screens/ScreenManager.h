#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "Screen.h"
#include "PlayScreen.h"
#include "../Presets/PresetManager.h"
#include "../framework/LCDCanvas.h"

class ScreenManager
{
private:
	static const uint8 MAX_ACTIVE_SCREEN_COUNT = 4;

public:
	ScreenManager(PresetManager *PresetManager)
		: m_PresetManager(PresetManager),
		  m_ActiveScreens{},
		  m_ActiveScreenCount(0)
	{
	}

	void Initialize(void)
	{
		m_ActiveScreens[m_ActiveScreenCount++] = new PlayScreen(m_PresetManager);
	}

	void Draw(LCDCanvas &Canvas)
	{
		m_ActiveScreens[m_ActiveScreenCount - 1]->Draw(Canvas);
	}

private:
	PresetManager *m_PresetManager;
	Screen *m_ActiveScreens[MAX_ACTIVE_SCREEN_COUNT];
	uint8 m_ActiveScreenCount;
};

#endif
#pragma GCC pop_options