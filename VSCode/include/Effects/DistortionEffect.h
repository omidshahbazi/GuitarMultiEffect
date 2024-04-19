#ifdef DISTORTION_EFFECT

#pragma once
#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Distortion.h"

template <typename T>
class DistortionEffect : public Effect<T>
{
public:
	DistortionEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Distortion(SampleRate),
		  m_GainPot(nullptr),
		  m_DrivePot(nullptr)
	{
		m_GainPot = ControlManager->CreatePotentiometer("Gain", AnalogPins::Pin0);
		m_GainPot->SetOnChangedListener(
			[&](float value)
			{
				m_Distortion.SetGain(value);
			});

		m_DrivePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin1);
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