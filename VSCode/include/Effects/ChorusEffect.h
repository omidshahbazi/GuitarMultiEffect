#ifdef CHORUS_EFFECT

#pragma once
#ifndef CHORUS_EFFECT_H
#define CHORUS_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Chorus.h"

template <typename T>
class ChorusEffect : public Effect<T>
{
public:
	ChorusEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Chorus(SampleRate),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", AnalogPins::Pin0);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<ChorusEffect *>(Context)->m_Chorus.SetWetRate(Value);
											}});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", AnalogPins::Pin1);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<ChorusEffect *>(Context)->m_Chorus.SetDepth(Math::Lerp(0.0, Chorus<T>::MAX_DEPTH, Value));
										  }});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin2);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<ChorusEffect *>(Context)->m_Chorus.SetRate(Math::Lerp(0.01, 4, Value));
										 }});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Chorus;
	}

private:
	Chorus<T> m_Chorus;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif