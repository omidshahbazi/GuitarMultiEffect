#ifdef TEST_EFFECT

#include "Effects/TestEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

TestEffect::TestEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Test(SampleRate)
{
	m_Pot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_Pot->SetOnChangedListener(
		[&](float value)
		{
			// m_Test.SetValue(value);
		});
}

IDSP *TestEffect::GetDSP(void)
{
	return &m_Test;
}

#endif