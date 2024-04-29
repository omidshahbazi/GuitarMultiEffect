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
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Looper(SampleRate, MAX_DELAY_TIME),
		  m_VolumePot(nullptr)
	{
		m_VolumePot = ControlManager->CreatePotentiometer("Volume", GPIOPins::Pin15);
		m_VolumePot->SetOnChangedListener(
			[&](float value)
			{
				m_Looper.SetVolume(value);
			});

		SetEnabled(true);

		if (Effect<T>::GetIsEnabledButtonOn())
			OnTurnedOn();
	}

protected:
	void SetEnabled(bool Value) override
	{
		Effect<T>::SetEnabled(true);
	}

	void OnTurnedOn(void) override
	{
		m_Looper.SetRecordMode();

		Effect<T>::SetLEDBlinkingEnabled(true);
	}

	void OnTurnedOff(float TurnedOnTime) override
	{
		Effect<T>::SetLEDBlinkingEnabled(false);

		// TODO: Due to the memory limitations, This cannot be done since it needs to be less than 1s which is a short amount of time and the max record time is 1s
		// if (TurnedOnTime < 1)
		// {
		// 	m_Looper.Clear();

		// 	return;
		// }

		m_Looper.SetReplayMode(Math::Min(MAX_DELAY_TIME, TurnedOnTime));
	}

	IDSP<T> *GetDSP(void)
	{
		return &m_Looper;
	}

private:
	Looper<T> m_Looper;
	Potentiometer *m_VolumePot;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif

#endif