#ifdef CHORUS_EFFECT

#include "Effects/ChorusEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

ChorusEffect::ChorusEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Chorus(SampleRate)
{
	m_DepthPot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DepthPot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetDepth(Math::Lerp(0.0, Chorus::MAX_DEPTH, value));
		});

	m_RatePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_RatePot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetRate(Math::Lerp(0.01, 4, value));
		});
}

IDSP *ChorusEffect::GetDSP(void)
{
	return &m_Chorus;
}

#endif