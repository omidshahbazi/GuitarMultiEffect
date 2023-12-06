#include "Effects/SustainEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

SustainEffect::SustainEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Sustain(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			// m_Sustain.SetFeedback(value);
			// m_Sustain.SetDelay(value); ???
		});
}

IDSP *SustainEffect::GetDSP(void)
{
	return &m_Sustain;
}