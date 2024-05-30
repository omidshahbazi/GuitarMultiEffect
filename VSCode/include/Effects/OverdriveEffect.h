#ifdef OVERDRIVE_EFFECT

#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Overdrive.h>

template <typename T, uint32 SampleRate>
class OverdriveEffect : public Effect<T, SampleRate>
{
public:
	OverdriveEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_DrivePot(nullptr),
		  m_GainPot(nullptr)
	{
		m_DrivePot = ControlManager->CreatePotentiometer("Drive", GPIOPins::Pin14);
		m_DrivePot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<OverdriveEffect *>(Context)->m_Overdrive.SetDrive(Value);
										  }});

		m_GainPot = ControlManager->CreatePotentiometer("Gain", GPIOPins::Pin15);
		m_GainPot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<OverdriveEffect *>(Context)->m_Overdrive.SetGain(Value);
										 }});
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return &m_Overdrive;
	}

private:
	Overdrive<T, SampleRate> m_Overdrive;
	Potentiometer *m_DrivePot;
	Potentiometer *m_GainPot;
};

#endif

#endif