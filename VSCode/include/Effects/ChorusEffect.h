#ifdef CHORUS_EFFECT

#pragma once
#ifndef CHORUS_EFFECT_H
#define CHORUS_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Chorus.h>

template <typename T>
class ChorusEffect : public Effect<T>
{
public:
	ChorusEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Chorus(SampleRate),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
		m_WetRatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Chorus.SetWetRate(value);
			});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin14);
		m_DepthPot->SetOnChangedListener(
			[&](float value)
			{
				m_Chorus.SetDepth(Math::Lerp(0.0, Chorus<T>::MAX_DEPTH, value));
			});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
		m_RatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Chorus.SetRate(Math::Lerp(0.01, 4, value));
			});
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