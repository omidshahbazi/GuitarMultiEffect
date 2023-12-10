#ifdef AUTO_WAH_EFFECT

#pragma once
#ifndef AUTO_WAH_EFFECT_H
#define AUTO_WAH_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/AutoWah.h>

class ControlManager;

class AutoWahEffect : public Effect
{
public:
	AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	AutoWah m_AutoWah;
};

#endif

#endif