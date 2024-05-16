#ifdef TREMOLO_EFFECT

#pragma once
#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Tremolo.h>

template <typename T>
class TremoloEffect : public Effect<T>
{
public:
	TremoloEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Tremolo(SampleRate),
		  m_RatePot(nullptr),
		  m_DepthPot(nullptr)
	{
		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin14);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<TremoloEffect *>(Context)->m_Tremolo.SetRate(Math::Lerp(0.01, 25, Value));
										 }});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin15);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<TremoloEffect *>(Context)->m_Tremolo.SetDepth(Value);
										  }});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Tremolo;
	}

private:
	Tremolo<T> m_Tremolo;
	Potentiometer *m_RatePot;
	Potentiometer *m_DepthPot;
};

#endif

#endif