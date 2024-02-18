#ifdef PHASER_EFFECT

#include "Effects/PhaserEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

PhaserEffect::PhaserEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Phaser(SampleRate)
{
	m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
	m_RatePot->SetOnChangedListener(
		[&](float value)
		{
			m_Phaser.SetRate(Math::Lerp(0.01, 25, value));
		});
}

IDSP *PhaserEffect::GetDSP(void)
{
	return &m_Phaser;
}

#endif