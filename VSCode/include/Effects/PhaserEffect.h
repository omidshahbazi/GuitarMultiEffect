#ifdef PHASER_EFFECT

#pragma once
#ifndef PHASER_EFFECT_H
#define PHASER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/Phaser.h>

class Potentiometer;

class PhaserEffect : public Effect
{
public:
	PhaserEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Phaser m_Phaser;
	Potentiometer *m_RatePot;
};

#endif

#endif