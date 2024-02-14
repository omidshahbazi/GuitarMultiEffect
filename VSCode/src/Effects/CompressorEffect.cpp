#ifdef COMPRESSOR_EFFECT

#include "Effects/CompressorEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>
#include <framework/include/Math.h>

CompressorEffect::CompressorEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Compressor(SampleRate),
	  m_RatioPot(nullptr)
{
	m_RatioPot = ControlManager->CreatePotentiometer("Ratio", GPIOPins::Pin15);
	m_RatioPot->SetOnChangedListener(
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