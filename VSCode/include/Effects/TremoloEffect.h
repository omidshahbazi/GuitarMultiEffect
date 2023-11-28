#pragma once
#ifndef TREMOLO_EFFECT_H
#define TREMOLO_EFFECT_H

#include "Effect.h"
#include "framework/include/DSP/Tremolo.h"

class ControlManager;
class Potentiometer;

class TremoloEffect : public Effect
{
public:
	TremoloEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Tremolo m_Tremolo;
	Potentiometer *m_DrivePot;
};

#endif