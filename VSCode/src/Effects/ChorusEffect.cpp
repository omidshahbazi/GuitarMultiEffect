#ifdef CHORUS_EFFECT

#include "Effects/ChorusEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

ChorusEffect::ChorusEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Chorus(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetDepth(value);
		});

	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetRate(value);
		});

	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin34);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetDelayTime(Math::Lerp(0, Chorus::MAX_DELAY_TIME, value));
		});
}

IDSP *ChorusEffect::GetDSP(void)
{
	return &m_Chorus;
}

#endif