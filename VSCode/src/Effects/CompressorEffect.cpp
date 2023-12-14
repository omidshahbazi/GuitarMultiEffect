#ifdef COMPRESSOR_EFFECT

#include "Effects/CompressorEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>
#include <framework/include/Math.h>

CompressorEffect::CompressorEffect(ControlManager *ControlManager)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Compressor.SetThreshold(Math::Lerp(-1.0, 1, value));
		});

	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin15);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			m_Compressor.SetRatio(Math::Lerp(0.001, 2, value));
		});
}

IDSP *CompressorEffect::GetDSP(void)
{
	return &m_Compressor;
}

#endif