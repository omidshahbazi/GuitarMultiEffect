#ifdef REVERB_EFFECT

#pragma once
#ifndef REVERB_EFFECT_H
#define REVERB_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Reverb.h>

template <typename T>
class ReverbEffect : public Effect<T>
{
public:
	ReverbEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Reverb(SampleRate, MAX_DELAY_TIME),
		  m_WetRatePot(nullptr),
		  m_DelayTimePot(nullptr),
		  m_FeedbackPot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
		m_WetRatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Reverb.SetWetRate(value);
			});

		m_DelayTimePot = ControlManager->CreatePotentiometer("Delay Time", GPIOPins::Pin14);
		m_DelayTimePot->SetOnChangedListener(
			[&](float value)
			{
				m_Reverb.SetDelayTime(Math::Lerp(0.0, MAX_DELAY_TIME, value));
			});

		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", GPIOPins::Pin15);
		m_FeedbackPot->SetOnChangedListener(
			[&](float value)
			{
				m_Reverb.SetFeedback(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Reverb;
	}

private:
	Reverb<T> m_Reverb;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DelayTimePot;
	Potentiometer *m_FeedbackPot;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif

#endif