#ifdef AUTO_WAH_EFFECT

#pragma once
#ifndef AUTO_WAH_EFFECT_H
#define AUTO_WAH_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/AutoWah.h>

template <typename T>
class AutoWahEffect : public Effect<T>
{
public:
	AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_AutoWah(SampleRate)
	{
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return reinterpret_cast<IDSP<T> *>(&m_AutoWah);
	}

private:
	AutoWah<T> m_AutoWah;
};

#endif

#endif