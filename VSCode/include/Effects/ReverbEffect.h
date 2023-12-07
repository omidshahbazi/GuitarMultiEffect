#pragma once
#ifndef REVERB_EFFECT_H
#define REVERB_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Reverb.h>

class ControlManager;
class Potentiometer;

class ReverbEffect : public Effect
{
public:
	ReverbEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Reverb m_Reverb;
	Potentiometer *m_DrivePot;
};

#endif