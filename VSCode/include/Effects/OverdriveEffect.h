#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Overdrive.h>

class ControlManager;
class Potentiometer;

class OverdriveEffect : public Effect
{
public:
	OverdriveEffect(ControlManager *ControlManager);

protected:
	IDSP *GetDSP(void);

private:
	Overdrive m_Overdrive;
	Potentiometer *m_DrivePot;
};

#endif