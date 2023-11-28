#include "Effects/TremoloEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

TremoloEffect::TremoloEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Tremolo(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Tremolo.SetRate(Math::Lerp(MIN_FREQUENCY, MAX_SAMPLE_RATE, value));
		});

	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Tremolo.SetDepth(value);
		});
}

IDSP *TremoloEffect::GetDSP(void)
{
	return &m_Tremolo;
}