#ifdef SUSTAIN_EFFECT

#include "Effects/SustainEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

SustainEffect::SustainEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Sustain(SampleRate)
{
	m_FeedbackPot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_FeedbackPot->SetOnChangedListener(
		[&](float value)
		{
			m_Sustain.SetFeedback(value);
		});
}

IDSP *SustainEffect::GetDSP(void)
{
	return &m_Sustain;
}

#endif