#ifdef REVERB_EFFECT

#include "Effects/ReverbEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

ReverbEffect::ReverbEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Reverb(SampleRate)
{
	m_DelayTimePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DelayTimePot->SetOnChangedListener(
		[&](float value)
		{
			m_Reverb.SetDelayTime(Math::Lerp(0.0, Reverb::MAX_DELAY_TIME, value));
		});

	m_FeedbackPot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_FeedbackPot->SetOnChangedListener(
		[&](float value)
		{
			m_Reverb.SetFeedback(value);
		});
}

IDSP *ReverbEffect::GetDSP(void)
{
	return &m_Reverb;
}

#endif