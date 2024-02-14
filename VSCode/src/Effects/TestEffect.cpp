#ifdef TEST_EFFECT

#include "Effects/TestEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

TestEffect::TestEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Test(SampleRate)
{
	m_Pot1 = ControlManager->CreatePotentiometer("High Tone", GPIOPins::Pin13);
	m_Pot1->SetOnChangedListener(
		[&](float value)
		{
			m_Test.SetHighTone(Math::Lerp(-20.0, 20, value));
		});

	m_Pot2 = ControlManager->CreatePotentiometer("Mid Tone", GPIOPins::Pin14);
	m_Pot2->SetOnChangedListener(
		[&](float value)
		{
			m_Test.SetMidTone(Math::Lerp(-20.0, 20, value));
		});

	m_Pot3 = ControlManager->CreatePotentiometer("Low Tone", GPIOPins::Pin15);
	m_Pot3->SetOnChangedListener(
		[&](float value)
		{
			m_Test.SetLowTone(Math::Lerp(-20.0, 20, value));
		});
}

IDSP *TestEffect::GetDSP(void)
{
	return &m_Test;
}

#endif