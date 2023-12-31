#ifdef TEST_EFFECT

#pragma once
#ifndef TEST_EFFECT_H
#define TEST_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Test.h>

class ControlManager;
class Potentiometer;

class TestEffect : public Effect
{
public:
	TestEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Test m_Test;
	Potentiometer *m_Pot;
};

#endif

#endif