#ifdef REVERB_EFFECT

#pragma once
#ifndef REVERB_EFFECT_H
#define REVERB_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Reverb.h>

template <typename T, uint32 SampleRate>
class ReverbEffect : public Effect<T, SampleRate>
{
private:
	static constexpr uint8 MAX_DELAY_TIME = 1;

public:
	ReverbEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_WetRatePot(nullptr),
		  m_DelayTimePot(nullptr),
		  m_FeedbackPot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<ReverbEffect *>(Context)->m_Reverb.SetWetRate(Value);
											}});

		m_DelayTimePot = ControlManager->CreatePotentiometer("Delay Time", GPIOPins::Pin14);
		m_DelayTimePot->SetOnChangedListener({this, [](void *Context, float Value)
											  {
												  static_cast<ReverbEffect *>(Context)->m_Reverb.SetDelayTime(Math::Lerp(0.0, MAX_DELAY_TIME, Value));
											  }});

		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", GPIOPins::Pin15);
		m_FeedbackPot->SetOnChangedListener({this, [](void *Context, float Value)
											 {
												 static_cast<ReverbEffect *>(Context)->m_Reverb.SetFeedback(Value);
											 }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Reverb;
	}

private:
	Reverb<T, SampleRate, MAX_DELAY_TIME> m_Reverb;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DelayTimePot;
	Potentiometer *m_FeedbackPot;
};

#endif

#endif