#include "Effects/AutoWahEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

AutoWahEffect::AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_AutoWah(SampleRate)
{
	Potentiometer *m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_AutoWah.SetFrequency_TOTUNE(Math::Lerp(MIN_FREQUENCY, MAX_FREQUENCY, value));
		});
}

IDSP *AutoWahEffect::GetDSP(void)
{
	return &m_AutoWah;
}