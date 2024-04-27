#ifdef DISTORTION_EFFECT

#pragma once
#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Distortion.h>

template <typename T>
class DistortionEffect : public Effect<T>
{
public:
	DistortionEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_Distortion(SampleRate),
		  m_RatePot(nullptr),
		  m_GainPot(nullptr)
	{
		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin14);
		m_RatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Distortion.SetRate(value);
			});

		m_GainPot = ControlManager->CreatePotentiometer("Gain", GPIOPins::Pin15);
		m_GainPot->SetOnChangedListener(
			[&](float value)
			{
				m_Distortion.SetGain(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Distortion;
	}

private:
	Distortion<T> m_Distortion;
	Potentiometer *m_RatePot;
	Potentiometer *m_GainPot;
};

#endif

#endif