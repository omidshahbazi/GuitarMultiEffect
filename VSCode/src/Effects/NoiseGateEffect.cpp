#include "Effects/NoiseGateEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

NoiseGateEffect::NoiseGateEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_NoiseGate(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			// m_NoiseGate.SetThreshold(value);
			m_NoiseGate.SetAttackTime(Math::Lerp(0.0, 1, value));
			// m_NoiseGate.SetReleaseTime(Math::Lerp(0.0, 2, value));
		});
}

IDSP *NoiseGateEffect::GetDSP(void)
{
	return &m_NoiseGate;
}