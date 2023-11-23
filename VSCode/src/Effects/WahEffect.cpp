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
			m_Wah.SetFrequency(Math::Lerp(200, 20000, value));
		});
}

IDSP *WahEffect::GetDSP(void)
{
	return &m_Wah;
}