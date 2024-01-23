#ifdef AUTO_WAH_EFFECT

#include "Effects/AutoWahEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>
#include <framework/include/Controls/Potentiometer.h>

AutoWahEffect::AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_AutoWah(SampleRate)
{
	Potentiometer *m_GainPot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_GainPot->SetOnChangedListener(
		[&](float value)
		{
			m_AutoWah.SetAttack(value);
		});

	Potentiometer *m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_AutoWah.SetRelease(value);
		});
}

IDSP *AutoWahEffect::GetDSP(void)
{
	return reinterpret_cast<IDSP *>(&m_AutoWah);
}

#endif