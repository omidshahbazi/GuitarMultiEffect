#ifdef NOISE_GATE_EFFECT

#include "Effects/NoiseGateEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

NoiseGateEffect::NoiseGateEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_NoiseGate(SampleRate),
	  m_ThresholdPot(nullptr)
{
	m_ThresholdPot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_ThresholdPot->SetOnChangedListener(
		[&](float value)
		{
			m_NoiseGate.SetThreshold(value);
		});
}

IDSP *NoiseGateEffect::GetDSP(void)
{
	return &m_NoiseGate;
}

#endif