#ifdef DISTORTION_EFFECT

#include "Effects/DistortionEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

DistortionEffect::DistortionEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Distortion(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Distortion.SetRate(value);
		});
}

IDSP *DistortionEffect::GetDSP(void)
{
	return &m_Distortion;
}

#endif