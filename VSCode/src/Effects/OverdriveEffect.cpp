#include "Effects/OverdriveEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

OverdriveEffect::OverdriveEffect(ControlManager *ControlManager)
{
	ControlManager->BindToPushButton(0,
									 [&]()
									 {
										 ToggleEnabled();
									 });

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