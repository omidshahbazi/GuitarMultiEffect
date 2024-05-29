#ifdef WAH_EFFECT

#pragma once
#ifndef WAH_EFFECT_H
#define WAH_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Wah.h"

template <typename T, uint32 SampleRate>
class WahEffect : public Effect<T, SampleRate>
{
public:
	WahEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_RatioPot(nullptr)
	{
		m_RatioPot = ControlManager->CreatePotentiometer("Ratio", AnalogPins::Pin0);
		m_RatioPot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<WahEffect *>(Context)->m_Wah.SetRatio(Value);
										  }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Wah;
	}

private:
	Wah<T, SampleRate> m_Wah;
	Potentiometer *m_RatioPot;
};

#endif

#endif