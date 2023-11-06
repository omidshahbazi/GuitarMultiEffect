#pragma once
#ifndef I_EFFECT_H
#define I_EFFECT_H

#include "framework/include/Common.h"

class IEffect
{
public:
	virtual void Process(const float *InBuffer, float *ProcessedBuffer, uint8 Count) = 0;
};

#endif