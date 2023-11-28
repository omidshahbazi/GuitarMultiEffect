#include "Effects/TestEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

TestEffect::TestEffect(ControlManager *ControlManager)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Test.SetValue(value);
		});
}

IDSP *TestEffect::GetDSP(void)
{
	return &m_Test;
}