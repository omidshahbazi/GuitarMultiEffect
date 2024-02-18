#ifdef TREMOLO_EFFECT

#include "Effects/TremoloEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

TremoloEffect::TremoloEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Tremolo(SampleRate),
	  m_RatePot(nullptr),
	  m_DepthPot(nullptr)
{
	m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin14);
	m_RatePot->SetOnChangedListener(
		[&](float value)
		{
			m_Tremolo.SetRate(Math::Lerp(0.01, 25, value));
		});

	m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin15);
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