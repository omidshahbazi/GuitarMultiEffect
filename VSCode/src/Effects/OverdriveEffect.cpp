#ifdef OVERDRIVE_EFFECT

#include "Effects/OverdriveEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

OverdriveEffect::OverdriveEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Overdrive(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Overdrive.SetDrive(value);
		});
}

IDSP *OverdriveEffect::GetDSP(void)
{
	return &m_Overdrive;
}

#endif