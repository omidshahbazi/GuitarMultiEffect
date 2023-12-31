#ifdef WAH_EFFECT

#include "Effects/WahEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

WahEffect::WahEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Wah(SampleRate)
{
	m_RatioPot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_RatioPot->SetOnChangedListener(
		[&](float value)
		{
			m_Wah.SetRatio(value);
		});
}

IDSP *WahEffect::GetDSP(void)
{
	return &m_Wah;
}

#endif