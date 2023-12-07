#pragma once
#ifndef WAH_EFFECT_H
#define WAH_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Wah.h>

class ControlManager;
class Potentiometer;

class WahEffect : public Effect
{
public:
	WahEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Wah m_Wah;
	Potentiometer *m_DrivePot;
};

#endif