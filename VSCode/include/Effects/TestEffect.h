#ifdef TEST_EFFECT

#pragma once
#ifndef TEST_EFFECT_H
#define TEST_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Test.h>

template <typename T>
class TestEffect : public Effect<T>
{
public:
	TestEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Overdrive(SampleRate),
		  m_DrivePot(nullptr),
		  m_GainPot(nullptr)
	{
		m_DrivePot = ControlManager->CreatePotentiometer("Drive", GPIOPins::Pin14);
		m_DrivePot->SetOnChangedListener(
			[&](float value)
			{
				m_Overdrive.SetDrive(value);
			});

		m_GainPot = ControlManager->CreatePotentiometer("Gain", GPIOPins::Pin15);
		m_GainPot->SetOnChangedListener(
			[&](float value)
			{
				m_Overdrive.SetGain(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Overdrive;
	}

private:
	Test<T> m_Overdrive;
	Potentiometer *m_DrivePot;
	Potentiometer *m_GainPot;
};

#endif

#endif