#ifdef DISTORTION_EFFECT

#pragma once
#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Distortion.h"

template <typename T, uint32 SampleRate>
class DistortionEffect : public Effect<T, SampleRate>
{
public:
	DistortionEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_RatePot(nullptr),
		  m_GainPot(nullptr)
	{
		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin0);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<DistortionEffect *>(Context)->m_Distortion.SetLevel(Value);
										 }});

		m_GainPot = ControlManager->CreatePotentiometer("Gain", AnalogPins::Pin1);
		m_GainPot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<DistortionEffect *>(Context)->m_Distortion.SetGain(Value);
										 }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Distortion;
	}

private:
	Distortion<T, SampleRate> m_Distortion;
	Potentiometer *m_RatePot;
	Potentiometer *m_GainPot;
};

#endif

#endif