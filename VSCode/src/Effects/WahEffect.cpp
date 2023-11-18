#include "Effects/WahEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

WahEffect::WahEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Wah(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Wah.SetFrequency(200 + (value * (20000 - 200)));
		});
}

IDSP *WahEffect::GetDSP(void)
{
	return &m_Wah;
}