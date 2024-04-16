#ifdef NOISE_GATE_EFFECT

#pragma once
#ifndef NOISE_GATE_EFFECT_H
#define NOISE_GATE_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/NoiseGate.h"

template <typename T>
class NoiseGateEffect : public Effect<T>
{
public:
	NoiseGateEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_NoiseGate(SampleRate),
		  m_ThresholdPot(nullptr)
	{
		m_ThresholdPot = ControlManager->CreatePotentiometer("Threshold", GPIOPins::Pin14);
		m_ThresholdPot->SetOnChangedListener(
			[&](float value)
			{
				m_NoiseGate.SetThreshold(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_NoiseGate;
	}

private:
	NoiseGate<T> m_NoiseGate;
	Potentiometer *m_ThresholdPot;
};

#endif

#endif