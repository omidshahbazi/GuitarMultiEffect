#pragma once
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "Screen.h"
#include "PlayScreen.h"
#include "PresetScreen.h"
#include "EffectScreen.h"
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
		  m_ActiveScreenIndex(-1)
	{
	}

	void Initialize(void)
	{
		Create<PlayScreen>(Screens::Play);
		Create<PresetScreen>(Screens::Preset);
		Create<EffectScreen>(Screens::Effect);
		Create<SaveScreen>(Screens::Save);

		SwitchTo(Screens::Play);

		m_ControlManager->SetBackButtonCallback({this,
												 [](void *Context, float HeldTime)
												 {
													 static_cast<ScreenManager *>(Context)->GoBack();
												 }});
	}

	void Render(LCDCanvas &Canvas)
	{
		m_ActiveScreens[m_ActiveScreenIndex]->Render(Canvas);
	}

	void SwitchTo(Screens Screen)
	{
		ASSERT(m_ActiveScreenIndex < MAX_ACTIVE_SCREEN_COUNT, "Out active screen array size");

		if (m_ActiveScreenIndex != -1)
			m_ActiveScreens[m_ActiveScreenIndex]->Deactivate();

		::Screen *screen = m_Screens[(uint8)Screen];

		++m_ActiveScreenIndex;

		ASSERT(m_ActiveScreens[m_ActiveScreenIndex] != screen, "The same screen wants to be activated");

		m_ActiveScreens[m_ActiveScreenIndex] = screen;

		screen->Activate();
	}

private:
	void GoBack(void)
	{
		if (m_ActiveScreenIndex == 0)
			return;

		m_ActiveScreens[m_ActiveScreenIndex]->Deactivate();
		m_ActiveScreens[m_ActiveScreenIndex] = nullptr;

		--m_ActiveScreenIndex;

		m_ActiveScreens[m_ActiveScreenIndex]->Activate();
	}

	template <typename T>
	void Create(Screens Screen)
	{
		T *screen = Memory::Allocate<T>(1, true);
		new (screen) T(m_PresetManager, m_ControlManager);

		reinterpret_cast<::Screen *>(screen)->SetOnSwitchScreen({this, [](void *Context, Screens Screen)
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
	int8 m_ActiveScreenIndex;
};

#endif