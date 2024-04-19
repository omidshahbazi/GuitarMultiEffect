#ifdef PHASER_EFFECT

#pragma once
#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Phaser.h"

template <typename T>
class PhaserEffect : public Effect<T>
{
public:
	PhaserEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Phaser(SampleRate),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", AnalogPins::Pin0);
		m_WetRatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Phaser.SetWetRate(value);
			});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", AnalogPins::Pin1);
		m_DepthPot->SetOnChangedListener(
			[&](float value)
			{
				m_Phaser.SetDepth(Math::Lerp(0.0, Phaser<T>::MAX_DEPTH, value));
			});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin2);
		m_RatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Phaser.SetRate(Math::Lerp(0.1, 3.7, value));
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Phaser;
	}

private:
	Phaser<T> m_Phaser;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif