#ifdef OVERDRIVE_EFFECT

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
		: Effect<T>(ControlManager),
		  m_GainPot(nullptr),
		  m_DrivePot(nullptr)
	{
		// m_GainPot = ControlManager->CreatePotentiometer("Gain", AnalogPins::Pin0);
		// m_GainPot->SetOnChangedListener(
		// 	[&](float value)
		// 	{
		// 		m_Overdrive.SetGain(value);
		// 	});

		// m_DrivePot = ControlManager->CreatePotentiometer("Drive", AnalogPins::Pin1);
		// m_DrivePot->SetOnChangedListener(
		// 	[&](float value)
		// 	{
		// 		m_Overdrive.SetDrive(value);
		// 	});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Overdrive;
	}

private:
	Overdrive<T> m_Overdrive;
	Potentiometer *m_GainPot;
	Potentiometer *m_DrivePot;
};

#endif

#endif