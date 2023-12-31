#ifdef SUSTAIN_EFFECT

#pragma once
#ifndef SUSTAIN_EFFECT_H
#define SUSTAIN_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Sustain.h>

class ControlManager;
class Potentiometer;

class SustainEffect : public Effect
{
public:
	SustainEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Sustain m_Sustain;
	Potentiometer *m_FeedbackPot;
};

#endif

#endif