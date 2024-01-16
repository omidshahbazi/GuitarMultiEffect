#ifdef OVERDRIVE_EFFECT

#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Overdrive.h>

class Potentiometer;

class OverdriveEffect : public Effect
{
public:
	OverdriveEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Overdrive m_Overdrive;
	Potentiometer *m_GainPot;
	Potentiometer *m_DrivePot;
};

#endif

#endif