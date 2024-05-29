#ifdef TREMOLO_EFFECT

#pragma once
#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Tremolo.h"

template <typename T, uint32 SampleRate>
class TremoloEffect : public Effect<T, SampleRate>
{
public:
	TremoloEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_RatePot(nullptr),
		  m_DepthPot(nullptr)
	{
		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin0);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<TremoloEffect *>(Context)->m_Tremolo.SetRate(Math::Lerp(0.01, 25, Value));
										 }});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", AnalogPins::Pin1);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<TremoloEffect *>(Context)->m_Tremolo.SetDepth(Value);
										  }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Tremolo;
	}

private:
	Tremolo<T, SampleRate> m_Tremolo;
	Potentiometer *m_RatePot;
	Potentiometer *m_DepthPot;
};

#endif

#endif