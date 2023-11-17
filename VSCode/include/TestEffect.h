#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include "framework/include/DSP/Test.h"

class ControlManager;
class Potentiometer;

class TestEffect : public Effect
{
public:
	TestEffect(ControlManager *ControlManager);

protected:
	IDSP *GetDSP(void);

private:
	Test m_Test;
	Potentiometer *m_DrivePot;
};

#endif