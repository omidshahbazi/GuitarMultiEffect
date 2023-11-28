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
			m_Overdrive.SetDrive(Math::Lerp(0, 100, value));
		});
}

IDSP *OverdriveEffect::GetDSP(void)
{
	return &m_Overdrive;
}