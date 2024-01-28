#ifdef SUSTAIN_EFFECT

#include "Effects/SustainEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>
#include <framework/include/Controls/Switch.h>

SustainEffect::SustainEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Sustain(SampleRate)
{
	m_FeedbackPot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_FeedbackPot->SetOnChangedListener(
		[&](float value)
		{
			m_Sustain.SetFeedback(value);
		});

	m_ActiveSwitch = ControlManager->CreateSwitch(GPIOPins::Pin21);
	m_ActiveSwitch->SetOnChangedListener(
		[&](bool value)
		{
			m_Sustain.SetActive(value);
		});

	m_WetSwitch = ControlManager->CreateSwitch(GPIOPins::Pin22);
	m_WetSwitch->SetOnChangedListener(
		[&](bool value)
		{
			m_Sustain.SetWet(value);
		});
}

IDSP *SustainEffect::GetDSP(void)
{
	return &m_Sustain;
}

#endif