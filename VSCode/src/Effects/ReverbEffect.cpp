#include "Effects/ReverbEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

ReverbEffect::ReverbEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Reverb(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
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