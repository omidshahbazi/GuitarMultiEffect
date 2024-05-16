#ifdef PHASER_EFFECT

#pragma once
#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Phaser.h>

template <typename T>
class PhaserEffect : public Effect<T>
{
public:
	PhaserEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Phaser(SampleRate),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
		m_WetRatePot->SetOnChangedListener({this, [](void *Context, float Value)
											{
												static_cast<PhaserEffect *>(Context)->m_Phaser.SetWetRate(Value);
											}});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin14);
		m_DepthPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<PhaserEffect *>(Context)->m_Phaser.SetDepth(Math::Lerp(0.0, Phaser<T>::MAX_DEPTH, Value));
										  }});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
		m_RatePot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<PhaserEffect *>(Context)->m_Phaser.SetRate(Math::Lerp(0.1, 3.7, Value));
										 }});
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