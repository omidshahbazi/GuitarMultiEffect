#pragma once
#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H

#include "Effect.h"

class DelayEffect : public Effect
{
protected:
	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
	}

private:
};

#endif