#ifdef LOOPER_EFFECT

#pragma once
#ifndef LOOPER_EFFECT_H
#define LOOPER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Looper.h>

class Switch;
class Potentiometer;

class LooperEffect : public Effect
{
public:
	LooperEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Looper m_Looper;
	Switch *m_ModeSwitch;
	Potentiometer *m_VolumePot;
};

#endif

#endif