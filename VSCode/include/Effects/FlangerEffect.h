#ifdef FLANGER_EFFECT

#pragma once
#ifndef FLANGER_EFFECT_H
#define FLANGER_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Flanger.h"

template <typename T, uint32 SampleRate>
class FlangerEffect : public Effect<T, SampleRate>
{
public:
	FlangerEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_FeedbackPot(nullptr),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", AnalogPins::Pin0);
		m_FeedbackPot->SetOnChangedListener({this, [](void *Context, float Value)
											 {
												 static_cast<FlangerEffect *>(Context)->m_Flanger.SetFeedback(Value);
											 }});

		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", AnalogPins::Pin1);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<FlangerEffect *>(Context)->m_Flanger.SetWetRate(Value);
											}});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", AnalogPins::Pin2);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<FlangerEffect *>(Context)->m_Flanger.SetDepth(Math::Lerp(0.0, Flanger<T, SampleRate>::MAX_DEPTH, Value));
										  }});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin3);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<FlangerEffect *>(Context)->m_Flanger.SetRate(Math::Lerp(0.01, 4, Value));
										 }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Flanger;
	}

private:
	Flanger<T, SampleRate> m_Flanger;
	Potentiometer *m_FeedbackPot;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif