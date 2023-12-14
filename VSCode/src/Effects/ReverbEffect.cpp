#ifdef REVERB_EFFECT

#include "Effects/ReverbEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

ReverbEffect::ReverbEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Reverb(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Reverb.SetDelayTime(Math::Lerp(0.0, Reverb::MAX_DELAY_TIME, value));
		});

	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DrivePot->SetOnChangedListener(
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