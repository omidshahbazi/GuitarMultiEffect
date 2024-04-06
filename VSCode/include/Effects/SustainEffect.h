#ifdef SUSTAIN_EFFECT

#pragma once
#ifndef SUSTAIN_EFFECT_H
#define SUSTAIN_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Sustain.h>

class Potentiometer;
class Switch;

class SustainEffect : public Effect
{
public:
	SustainEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Sustain m_Sustain;
	Potentiometer *m_FeedbackPot;
	Switch *m_ActiveSwitch;
	Switch *m_WetSwitch;
};

#endif

#endif