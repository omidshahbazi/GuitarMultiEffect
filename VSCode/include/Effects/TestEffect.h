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
		: Effect<T>(ControlManager),
		  m_Test(SampleRate)
	{
		m_Pot1 = ControlManager->CreatePotentiometer("High Tone", GPIOPins::Pin13);
		m_Pot1->SetOnChangedListener(
			[&](float value)
			{
				m_Test.SetHighTone(Math::Lerp(-20.0, 20, value));
			});

		m_Pot2 = ControlManager->CreatePotentiometer("Mid Tone", GPIOPins::Pin14);
		m_Pot2->SetOnChangedListener(
			[&](float value)
			{
				m_Test.SetMidTone(Math::Lerp(-20.0, 20, value));
			});

		m_Pot3 = ControlManager->CreatePotentiometer("Low Tone", GPIOPins::Pin15);
		m_Pot3->SetOnChangedListener(
			[&](float value)
			{
				m_Test.SetLowTone(Math::Lerp(-20.0, 20, value));
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Test;
	}

private:
	Test<T> m_Test;
	Potentiometer *m_Pot1;
	Potentiometer *m_Pot2;
	Potentiometer *m_Pot3;
};

#endif

#endif