#pragma once
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "PlayScreen.h"
#include "PresetScreen.h"
#include "LevelScreen.h"
#include "RenameScreen.h"
#include "EffectScreen.h"
#include "RhythmScreen.h"
#include "SaveScreen.h"
#include "LooperScreen.h"
// #include "TunerScreen.h"
#include "../framework/LCDCanvas.h"

class ScreenManager
{
private:
	static constexpr uint8 MAX_ACTIVE_SCREEN_COUNT = 10;

public:
	ScreenManager(PresetManager *PresetManager, ControlManager *ControlManager)
		: m_PresetManager(PresetManager),
		  m_ControlManager(ControlManager),
		  m_Screens{},
		  m_ScreensHistory{},
		  m_ActiveScreenIndex(-1)
	{
	}

	void Initialize(void)
	{
		Create<PlayScreen>(Screens::Play);
		Create<PresetScreen>(Screens::Preset);
		Create<LevelScreen>(Screens::Level);
		Create<RenameScreen>(Screens::Rename);
		Create<EffectScreen>(Screens::Effect);
		Create<RhythmScreen>(Screens::Rhythm);
		Create<SaveScreen>(Screens::Save);
		Create<LooperScreen>(Screens::Looper);
		// Create<TunerScreen>(Screens::Tuner);

		SwitchTo(Screens::Play);

		static auto checkAnSwitchToLooperScreen = [this]()
		{
			if (!(m_IsVariation2On && m_IsVariation3On))
				return;

			SwitchTo(Screens::Looper);

			m_IsVariation2On = false;
			m_IsVariation3On = false;
		};

		m_ControlManager->SetSaveButtonCallback({this,
												 [](void *Context, float HeldTime)
												 {
													 static_cast<ScreenManager *>(Context)->SwitchTo(Screens::Save);
												 }});

		m_ControlManager->SetBackButtonTurnedOffCallback({this,
														  [](void *Context, float HeldTime)
														  {
															  static_cast<ScreenManager *>(Context)->GoBack();
														  }});

		m_ControlManager->SetVariation2ButtonTurnedOnCallback({this,
															   [](void *Context)
															   {
																   static_cast<ScreenManager *>(Context)->m_IsVariation2On = true;

																   checkAnSwitchToLooperScreen();
															   }});

		m_ControlManager->SetVariation2ButtonTurnedOffCallback({this,
																[](void *Context, float HeldTime)
																{
																	static_cast<ScreenManager *>(Context)->m_IsVariation2On = false;
																}});

		m_ControlManager->SetVariation3ButtonTurnedOnCallback({this,
															   [](void *Context)
															   {
																   static_cast<ScreenManager *>(Context)->m_IsVariation3On = true;

																   checkAnSwitchToLooperScreen();
															   }});

		m_ControlManager->SetVariation2ButtonTurnedOffCallback({this,
																[](void *Context, float HeldTime)
																{
																	static_cast<ScreenManager *>(Context)->m_IsVariation3On = false;
																}});

		m_ControlManager->SetVariation3ButtonHoldCallback({this,
														   [](void *Context, float HeldTime)
														   {
															   if (HeldTime < 2)
																   return;

															   static_cast<ScreenManager *>(Context)->SwitchTo(Screens::Rhythm);
														   }});
	}

	void Update(void)
	{
		if (m_ActiveScreenIndex == -1)
			return;

		m_ScreensHistory[m_ActiveScreenIndex]->Update();
	}

	void Render(LCDCanvas &Canvas)
	{
		if (m_ActiveScreenIndex == -1)
			return;

		m_ScreensHistory[m_ActiveScreenIndex]->Render(Canvas);
	}

	void ProcessAudioBuffer(const SampleType *const InputBuffer, const SampleType *const OutputBuffer, uint8 Count)
	{
		if (m_ActiveScreenIndex == -1)
			return;

		m_ScreensHistory[m_ActiveScreenIndex]->ProcessAudioBuffer(InputBuffer, OutputBuffer, Count);
	}

private:
	void SwitchTo(Screens Screen)
	{
		ASSERT(m_ActiveScreenIndex < MAX_ACTIVE_SCREEN_COUNT, "Out of active screen array size");

		::Screen *screen = m_Screens[(uint8)Screen];

		if (m_ActiveScreenIndex != -1)
		{
			if (m_ScreensHistory[m_ActiveScreenIndex] == screen)
				return;

			m_ScreensHistory[m_ActiveScreenIndex]->Deactivate();
		}

		++m_ActiveScreenIndex;

		m_ScreensHistory[m_ActiveScreenIndex] = screen;

		screen->Activate();
	}

	void GoBack(void)
	{
		if (m_ActiveScreenIndex == 0)
			return;

		m_ScreensHistory[m_ActiveScreenIndex]->Deactivate();
		m_ScreensHistory[m_ActiveScreenIndex] = nullptr;

		--m_ActiveScreenIndex;

		m_ScreensHistory[m_ActiveScreenIndex]->Activate();
	}

	void ClearHistory(void)
	{
		for (uint8 i = 0; i <= m_ActiveScreenIndex; ++i)
		{
			m_ScreensHistory[i]->Deactivate();
			m_ScreensHistory[i] = nullptr;
		}

		m_ActiveScreenIndex = -1;
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

		reinterpret_cast<::Screen *>(screen)->SetOnClearScreensHistory({this, [](void *Context)
																		{
																			reinterpret_cast<ScreenManager *>(Context)->ClearHistory();
																		}});

		m_Screens[(uint8)Screen] = screen;
	}

private:
	PresetManager *m_PresetManager;
	ControlManager *m_ControlManager;
	Screen *m_Screens[(uint8)Screens::COUNT];
	Screen *m_ScreensHistory[MAX_ACTIVE_SCREEN_COUNT];
	int8 m_ActiveScreenIndex;

	bool m_IsVariation2On;
	bool m_IsVariation3On;
};

#endif