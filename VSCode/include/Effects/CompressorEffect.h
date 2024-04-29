#ifdef COMPRESSOR_EFFECT

#pragma once
#ifndef COMPRESSOR_EFFECT_H
#define COMPRESSOR_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Compressor.h>
#include <framework/include/DSP/Math.h>

template <typename T>
class CompressorEffect : public Effect<T>
{
public:
	CompressorEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Compressor(SampleRate),
		  m_RatioPot(nullptr),
		  m_ThresholdPot(nullptr)
	{
		m_RatioPot = ControlManager->CreatePotentiometer("Attack", GPIOPins::Pin13);
		m_RatioPot->SetOnChangedListener(
			[&](float value)
			{
				m_Compressor.SetAttackTime(Math::Lerp(0.001, 10, value));
			});

		m_RatioPot = ControlManager->CreatePotentiometer("Release", GPIOPins::Pin14);
		m_RatioPot->SetOnChangedListener(
			[&](float value)
			{
				m_Compressor.SetReleaseTime(Math::Lerp(0.001, 10, value));
			});

		m_RatioPot = ControlManager->CreatePotentiometer("Ratio", GPIOPins::Pin15);
		m_RatioPot->SetOnChangedListener(
			[&](float value)
			{
				m_Compressor.SetRatio(Math::Lerp(1.0, 40, value));
			});

		m_ThresholdPot = ControlManager->CreatePotentiometer("Threshold", GPIOPins::Pin34);
		m_ThresholdPot->SetOnChangedListener(
			[&](float value)
			{
				m_Compressor.SetThreshold(Math::Lerp(-80.0, 0, value));
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
	Potentiometer *m_ThresholdPot;
};

#endif

#endif