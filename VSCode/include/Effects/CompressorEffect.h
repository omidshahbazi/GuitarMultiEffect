#ifdef COMPRESSOR_EFFECT

#pragma once
#ifndef COMPRESSOR_EFFECT_H
#define COMPRESSOR_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Compressor.h"
#include "../framework/DSP/Math.h"

template <typename T>
class CompressorEffect : public Effect<T>
{
public:
	CompressorEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Compressor(SampleRate),
		  m_RatioPot(nullptr)
	{
		m_RatioPot = ControlManager->CreatePotentiometer("Ratio", AnalogPins::Pin0);
		m_RatioPot->SetOnChangedListener(
			[&](float value)
			{
				m_Compressor.SetRatio(Math::Lerp(0.001, 2, value));
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Compressor;
	}

private:
	Compressor<T> m_Compressor;
	Potentiometer *m_RatioPot;
};

#endif

#endif