#include "Effects/OverdriveEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

OverdriveEffect::OverdriveEffect(ControlManager *ControlManager)
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