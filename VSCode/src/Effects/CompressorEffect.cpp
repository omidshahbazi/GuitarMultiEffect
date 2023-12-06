#include "Effects/CompressorEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

CompressorEffect::CompressorEffect(ControlManager *ControlManager, uint32 SampleRate)
	: m_Compressor(SampleRate)
{
	m_DrivePot = ControlManager->CreatePotentiometer(GPIOPins::Pin14);
	m_DrivePot->SetOnChangedListener(
		[&](float value)
		{
			// m_Compressor.SetThreshold(value);
			// m_Compressor.SetRatio(value);
		});
}

IDSP *CompressorEffect::GetDSP(void)
{
	return &m_Compressor;
}