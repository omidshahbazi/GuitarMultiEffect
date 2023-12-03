#pragma once
#ifndef WAH_EFFECT_H
#define WAH_EFFECT_H

#include "Effect.h"
#include "framework/include/DSP/Wah.h"

class ControlManager;
class Potentiometer;

// Cry-Baby 175Hz - 2500Hz 7.9
// Boutique 100Hz - 5000Hz 8-10
// Full Range 20Hz - 20kHz 7.9
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