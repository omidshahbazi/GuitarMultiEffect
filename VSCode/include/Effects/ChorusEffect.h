#ifdef CHORUS_EFFECT

#pragma once
#ifndef CHORUS_EFFECT_H
#define CHORUS_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Chorus.h>

class Potentiometer;

class ChorusEffect : public Effect
{
public:
	ChorusEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Chorus m_Chorus;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif