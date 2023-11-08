#pragma once
#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H

#include "IEffect.h"

class DelayEffect : public IEffect
{
public:
	void Process(float *Buffer, uint16 Count) override
	{
	}

private:
};

#endif