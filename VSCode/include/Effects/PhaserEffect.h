#ifdef PHASER_EFFECT

#pragma once
#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Phaser.h"

template <typename T, uint32 SampleRate>
class PhaserEffect : public Effect<T, SampleRate>
{
public:
	PhaserEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", AnalogPins::Pin0);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<PhaserEffect *>(Context)->m_Phaser.SetWetRate(Value);
											}});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", AnalogPins::Pin1);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<PhaserEffect *>(Context)->m_Phaser.SetDepth(Math::Lerp(0.0, 100, Value));
										  }});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", AnalogPins::Pin2);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<PhaserEffect *>(Context)->m_Phaser.SetRate(Math::Lerp(0.1, 3.7, Value));
										 }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Phaser;
	}

private:
	Phaser<T, SampleRate> m_Phaser;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif