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

		// m_VolumePot = ControlManager->CreatePotentiometer("Volume", AnalogPins::Pin0);
		// m_VolumePot->SetOnChangedListener(
		// 	[&](float value)
		// 	{
		// 		m_Looper.SetVolume(value);
		// 	});
	}

	// LooperEffect(ControlManager *ControlManager, uint32 SampleRate)
	// 	: Effect<T>(ControlManager, GPIOPins::Pin10, GPIOPins::Pin11, GPIOPins::Pin29),
	// 	  m_Looper(SampleRate, MAX_DELAY_TIME),
	// 	  m_VolumePot(nullptr)
	// {
	// 	Effect<T>::SetEnabled(true);

	// 	m_VolumePot = ControlManager->CreatePotentiometer("Volume", AnalogPins::Pin11);
	// 	m_VolumePot->SetOnChangedListener(
	// 		[&](float value)
	// 		{
	// 			m_Looper.SetVolume(value);
	// 		});
	// }

protected:
	void OnButtonDown(void) override
	{
		m_Looper.SetRecordMode();

		Effect<T>::ChangeEnabledLEDBlinkingRate(2);
	}

	void OnButtonUp(float HeldTime) override
	{
		Effect<T>::ChangeEnabledLEDBlinkingRate();

		if (HeldTime < 2)
		{
			m_Looper.Clear();

			return;
		}

		m_Looper.SetReplayMode(Math::Min(MAX_DELAY_TIME, HeldTime));
	}

	IDSP<T> *GetDSP(void)
	{
		return &m_Looper;
	}

private:
	Looper<T> m_Looper;
	Potentiometer *m_VolumePot;

	static constexpr float MAX_DELAY_TIME = 300;
};

#endif

#endif