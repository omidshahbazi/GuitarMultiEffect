#pragma once
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "Screen.h"
#include "PlayScreen.h"
#include "PresetScreen.h"
#include "SaveScreen.h"
#include "../framework/LCDCanvas.h"

class ScreenManager
{
private:
	static constexpr uint8 MAX_ACTIVE_SCREEN_COUNT = 4;

public:
	ScreenManager(PresetManager *PresetManager, ControlManager *ControlManager)
		: m_PresetManager(PresetManager),
		  m_ControlManager(ControlManager),
		  m_Screens{},
		  m_ActiveScreens{},
		  m_ActiveScreenCount(0)
	{
	}

	void Initialize(void)
	{
		Create<PlayScreen>(Screens::Play);
		Create<PresetScreen>(Screens::Preset);
		Create<SaveScreen>(Screens::Save);

		SwitchTo(Screens::Play);
	}

	void Render(LCDCanvas &Canvas)
	{
		m_ActiveScreens[m_ActiveScreenCount - 1]->Render(Canvas);
	}

	void SwitchTo(Screens Screen)
	{
		ASSERT(m_ActiveScreenCount < MAX_ACTIVE_SCREEN_COUNT, "Out active screen array size");

		if (m_ActiveScreenCount != 0)
			m_ActiveScreens[m_ActiveScreenCount - 1]->Deactivate();

		::Screen *screen = m_Screens[(uint8)Screen];

		ASSERT(m_ActiveScreens[m_ActiveScreenCount - 1] != screen, "The same screen wants to be activated");

		m_ActiveScreens[m_ActiveScreenCount++] = screen;

		screen->Activate();
	}

	void GoBack(void)
	{
		if (m_ActiveScreenCount == 0)
			return;

		m_ActiveScreens[m_ActiveScreenCount - 1]->Deactivate();

		m_ActiveScreens[--m_ActiveScreenCount]->Activate();
	}

private:
	template <typename T>
	void Create(Screens Screen)
	{
		T *screen = Memory::Allocate<T>(1, true);
		new (screen) T(m_PresetManager, m_ControlManager);

		reinterpret_cast<::Screen *>(screen)->SetOnSwitchChange({this, [](void *Context, Screens Screen)
																 {
																	 reinterpret_cast<ScreenManager *>(Context)->SwitchTo(Screen);
																 }});

		m_Screens[(uint8)Screen] = screen;
	}

private:
	PresetManager *m_PresetManager;
	ControlManager *m_ControlManager;
	Screen *m_Screens[(uint8)Screens::COUNT];
	Screen *m_ActiveScreens[MAX_ACTIVE_SCREEN_COUNT];
	uint8 m_ActiveScreenCount;
};

#endif