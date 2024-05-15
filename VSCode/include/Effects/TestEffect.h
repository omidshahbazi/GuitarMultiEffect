#ifdef TEST_EFFECT

#pragma once
#ifndef TEST_EFFECT_H
#define TEST_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/Test.h"

template <typename T>
class TestEffect : public Effect<T>
{
public:
	TestEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12),
		  m_Test(SampleRate)
	{
		m_Pot1 = ControlManager->CreatePotentiometer("Pot 0", AnalogPins::Pin0);
		m_Pot1->SetOnChangedListener({this, [](void *Context, float Value)
									  {
										  static_cast<TestEffect *>(Context)->m_Test.SetDrive(Value);
									  }});

		m_Pot2 = ControlManager->CreatePotentiometer("Pot 1", AnalogPins::Pin1);
		m_Pot2->SetOnChangedListener({this, [](void *Context, float Value)
									  {
										  static_cast<TestEffect *>(Context)->m_Test.SetGain(Value);
									  }});
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
};

#endif

#endif