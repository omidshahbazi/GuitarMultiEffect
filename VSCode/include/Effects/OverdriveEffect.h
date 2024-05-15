#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Overdrive.h"

template <typename T>
class OverdriveEffect : public Effect<T>
{
public:
	OverdriveEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Overdrive(SampleRate),
		  m_DrivePot(nullptr),
		  m_GainPot(nullptr)
	{
		m_DrivePot = ControlManager->CreatePotentiometer("Drive", AnalogPins::Pin0);
		m_DrivePot->SetOnChangedListener({this, [](void *Context, float Value)
										  {
											  static_cast<OverdriveEffect *>(Context)->m_Overdrive.SetDrive(Value);
										  }});

		m_GainPot = ControlManager->CreatePotentiometer("Gain", AnalogPins::Pin1);
		m_GainPot->SetOnChangedListener({this, [](void *Context, float Value)
										 {
											 static_cast<OverdriveEffect *>(Context)->m_Overdrive.SetGain(Value);
										 }});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Overdrive;
	}

private:
	Overdrive<T> m_Overdrive;
	Potentiometer *m_DrivePot;
	Potentiometer *m_GainPot;
};

#endif