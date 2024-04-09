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
		: Effect<T>(ControlManager),
		  m_Tremolo(SampleRate),
		  m_RatePot(nullptr),
		  m_DepthPot(nullptr)
	{
		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin14);
		m_RatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Tremolo.SetRate(Math::Lerp(0.01, 25, value));
			});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin15);
		m_DepthPot->SetOnChangedListener(
			[&](float value)
			{
				m_Tremolo.SetDepth(value);
			});
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