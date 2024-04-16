#ifdef LOOPER_EFFECT

#pragma once
#ifndef LOOPER_EFFECT_H
#define LOOPER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Looper.h>

template <typename T>
class LooperEffect : public Effect<T>
{
public:
	LooperEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_Looper(SampleRate),
		  m_ModeSwitch(nullptr),
		  m_VolumePot(nullptr)
	{
		// TODO: this needs to set different modes using a single switch or probably two?

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
};

#endif

#endif