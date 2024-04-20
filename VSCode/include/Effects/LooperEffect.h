#ifdef LOOPER_EFFECT

#pragma once
#ifndef LOOPER_EFFECT_H
#define LOOPER_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Looper.h"

template <typename T>
class LooperEffect : public Effect<T>
{
public:
	LooperEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Looper(SampleRate, MAX_DELAY_TIME),
		  m_VolumePot(nullptr)
	{
		Effect<T>::SetEnabled(true);

		// TODO: this needs to set different modes using a single switch or probably two?

		// m_VolumePot = ControlManager->CreatePotentiometer("Volume", AnalogPins::Pin0);
		// m_VolumePot->SetOnChangedListener(
		// 	[&](float value)
		// 	{
		// 		m_Looper.SetVolume(value);
		// 	});
	}

protected:
	void OnButtonDown(void) override
	{
		m_Looper.SetMode(Looper<T>::Modes::Record);
	}

	void OnButtonUp(float HeldTime) override
	{
		// if (HeldTime < 3)
		// 	CLEAR

		m_Looper.SetMode(Looper<T>::Modes::Replay);
	}

	IDSP<T> *GetDSP(void)
	{
		return &m_Looper;
	}

private:
	Looper<T> m_Looper;
	Potentiometer *m_VolumePot;

	static constexpr float MAX_DELAY_TIME = 5;
};

#endif

#endif