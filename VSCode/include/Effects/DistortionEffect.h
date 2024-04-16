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
		  m_GainPot(nullptr),
		  m_DrivePot(nullptr)
	{
		m_GainPot = ControlManager->CreatePotentiometer("Gain", GPIOPins::Pin14);
		m_GainPot->SetOnChangedListener(
			[&](float value)
			{
				m_Distortion.SetGain(value);
			});

		m_DrivePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
		m_DrivePot->SetOnChangedListener(
			[&](float value)
			{
				m_Distortion.SetRate(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Distortion;
	}

private:
	Distortion<T> m_Distortion;
	Potentiometer *m_GainPot;
	Potentiometer *m_DrivePot;
};

#endif

#endif