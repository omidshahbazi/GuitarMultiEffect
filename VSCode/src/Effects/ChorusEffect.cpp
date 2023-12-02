#include "Effects/ChorusEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

ChorusEffect::ChorusEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Chorus(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Chorus.SetDelayTime(value);
		});
}

IDSP *ChorusEffect::GetDSP(void)
{
	return &m_Chorus;
}