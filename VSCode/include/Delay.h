#pragma once
#ifndef DELAY_H
#define DELAY_H

#include "IEffect.h"

class Delay : public IEffect
{
public:
	void Process(float *Buffer, uint16 Count) override
	{
	}

private:
};

#endif