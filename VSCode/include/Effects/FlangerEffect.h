#ifdef FLANGER_EFFECT

#pragma once
#ifndef FLANGER_EFFECT_H
#define FLANGER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Flanger.h>

template <typename T>
class FlangerEffect : public Effect<T>
{
public:
	FlangerEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Flanger(SampleRate),
		  m_FeedbackPot(nullptr),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", GPIOPins::Pin13);
		m_FeedbackPot->SetOnChangedListener({this, [](void *Context, float Value)
											 {
												 static_cast<FlangerEffect *>(Context)->m_Flanger.SetFeedback(Value);
											 }});

		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin14);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<FlangerEffect *>(Context)->m_Flanger.SetWetRate(Value);
											}});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin15);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<FlangerEffect *>(Context)->m_Flanger.SetDepth(Math::Lerp(0.0, Flanger<T>::MAX_DEPTH, Value));
										  }});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin34);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<FlangerEffect *>(Context)->m_Flanger.SetRate(Math::Lerp(0.01, 4, Value));
										 }});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Flanger;
	}

private:
	Flanger<T> m_Flanger;
	Potentiometer *m_FeedbackPot;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif