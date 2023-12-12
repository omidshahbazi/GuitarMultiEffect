#ifdef DISTORTION_EFFECT

#pragma once
#ifndef DISTORTION_EFFECT_H
#define DISTORTION_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Distortion.h>

class ControlManager;
class Potentiometer;

class DistortionEffect : public Effect
{
public:
	DistortionEffect(ControlManager *ControlManager);

protected:
	IDSP *GetDSP(void);

private:
	Distortion m_Distortion;
	Potentiometer *m_DrivePot;
};

#endif

#endif