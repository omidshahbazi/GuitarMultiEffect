#ifdef LOOPER_EFFECT

#pragma once
#ifndef LOOPER_EFFECT_H
#define LOOPER_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Looper.h"

template <typename T, uint32 SampleRate>
class LooperEffect : public Effect<T, SampleRate>
{
private:
	static constexpr uint8 MAX_DELAY_TIME = 300;

public:
	LooperEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin10, GPIOPins::Pin11, GPIOPins::Pin29),
		  m_VolumePot(nullptr)
	{
		Effect<T, SampleRate>::SetEnabled(true);

		m_VolumePot = ControlManager->CreatePotentiometer("Volume", AnalogPins::Pin11);
		m_VolumePot->SetOnChangedListener({this, [](void *Context, float Value)
										   {
											   static_cast<LooperEffect *>(Context)->m_Looper.SetVolume(Value);
										   }});
	}

protected:
	void OnButtonDown(void) override
	{
		m_Looper.SetRecordMode();

		Effect<T, SampleRate>::ChangeEnabledLEDBlinkingRate(2);
	}

	void OnButtonUp(float HeldTime) override
	{
		Effect<T, SampleRate>::ChangeEnabledLEDBlinkingRate();

		if (HeldTime < 2)
		{
			m_Looper.Clear();

			return;
		}

		m_Looper.SetReplayMode(Math::Min(MAX_DELAY_TIME, HeldTime));
	}

	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Looper;
	}

private:
	Looper<T, SampleRate, MAX_DELAY_TIME> m_Looper;
	Potentiometer *m_VolumePot;
};

#endif

#endif