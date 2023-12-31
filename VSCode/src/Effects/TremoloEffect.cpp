#ifdef TREMOLO_EFFECT

#include "Effects/TremoloEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

TremoloEffect::TremoloEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Tremolo(SampleRate)
{
	m_RatePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_RatePot->SetOnChangedListener(
		[&](float value)
		{
			m_Tremolo.SetRate(value);
		});

	m_DepthPot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DepthPot->SetOnChangedListener(
		[&](float value)
		{
			m_Tremolo.SetDepth(value);
		});
}

IDSP *TremoloEffect::GetDSP(void)
{
	return &m_Tremolo;
}

#endif