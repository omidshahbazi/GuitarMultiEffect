#ifdef OVERDRIVE_EFFECT

#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Overdrive.h>

template <typename T>
class OverdriveEffect : public Effect<T>
{
public:
	OverdriveEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_DrivePot(nullptr)
	{
		m_DrivePot = ControlManager->CreatePotentiometer("Drive", GPIOPins::Pin15);
		m_DrivePot->SetOnChangedListener(
			[&](float value)
			{
				m_Overdrive.SetDrive(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Overdrive;
	}

private:
	Overdrive<T> m_Overdrive;
	Potentiometer *m_DrivePot;
};

#endif

#endif