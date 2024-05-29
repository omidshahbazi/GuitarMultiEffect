#ifdef AUTO_WAH_EFFECT

#pragma once
#ifndef AUTO_WAH_EFFECT_H
#define AUTO_WAH_EFFECT_H

#include "Effect.h"
#include "../framework/DSP/DSPs/AutoWah.h"

template <typename T, uint32 SampleRate>
class AutoWahEffect : public Effect<T, SampleRate>
{
public:
	AutoWahEffect(ControlManager *ControlManager)
		: Effect<T, SampleRate>(ControlManager, GPIOPins::Pin0, GPIOPins::Pin1, GPIOPins::Pin12)
	{
	}

protected:
	IDSP<T, SampleRate> *GetDSP(void)
	{
		return reinterpret_cast<IDSP<T, SampleRate> *>(&m_AutoWah);
	}

private:
	AutoWah<T, SampleRate> m_AutoWah;
};

#endif

#endif