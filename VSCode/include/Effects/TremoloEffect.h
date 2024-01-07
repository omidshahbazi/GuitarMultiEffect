#ifdef TREMOLO_EFFECT

#pragma once
#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Tremolo.h>

class Potentiometer;

class TremoloEffect : public Effect
{
public:
	TremoloEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Tremolo m_Tremolo;
	Potentiometer *m_RatePot;
	Potentiometer *m_DepthPot;
};

#endif

#endif