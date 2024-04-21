#ifdef LOOPER_EFFECT

#pragma once
#ifndef LOOPER_EFFECT_H
#define LOOPER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Looper.h>

// TODO: Due to the memory limitations, I've left this effect uncompleted on ESP32-A1S, so the controls here are not well configured
template <typename T>
class LooperEffect : public Effect<T>
{
public:
	LooperEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_Looper(SampleRate, MAX_DELAY_TIME),
		  m_ModeSwitch(nullptr),
		  m_VolumePot(nullptr)
	{
		m_ModeSwitch = ControlManager->CreateSwitch("Mode", GPIOPins::Pin21);
		m_ModeSwitch->SetOnStateChangedListener(
			[&](bool value)
			{
				m_Looper.SetMode(value ? Looper<T>::Modes::Record : Looper<T>::Modes::Replay);
			});

		m_VolumePot = ControlManager->CreatePotentiometer("Volume", GPIOPins::Pin15);
		m_VolumePot->SetOnChangedListener(
			[&](float value)
			{
				m_Looper.SetVolume(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Looper;
	}

private:
	Looper<T> m_Looper;
	Switch *m_ModeSwitch;
	Potentiometer *m_VolumePot;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif

#endif