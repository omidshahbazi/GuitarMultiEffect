#pragma once
#ifndef NOISE_GATE_EFFECT_H
#define NOISE_GATE_EFFECT_H

#include "Effect.h"
#include "framework/include/DSP/NoiseGate.h"

class ControlManager;
class Potentiometer;

class NoiseGateEffect : public Effect
{
public:
	NoiseGateEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	NoiseGate m_NoiseGate;
	Potentiometer *m_DrivePot;
};

#endif