#ifdef FLANGER_EFFECT

#pragma once
#ifndef FLANGER_EFFECT_H
#define FLANGER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Flanger.h>

class Potentiometer;

class FlangerEffect : public Effect
{
public:
	FlangerEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Flanger m_Flanger;
	Potentiometer *m_FeedbackPot;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif