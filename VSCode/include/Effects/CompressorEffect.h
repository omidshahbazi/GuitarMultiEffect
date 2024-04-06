#ifdef COMPRESSOR_EFFECT

#pragma once
#ifndef COMPRESSOR_EFFECT_H
#define COMPRESSOR_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Compressor.h>

class Potentiometer;

class CompressorEffect : public Effect
{
public:
	CompressorEffect(ControlManager *ControlManager, uint32 SampleRate);

protected:
	IDSP *GetDSP(void);

private:
	Compressor m_Compressor;
	Potentiometer *m_RatioPot;
};

#endif

#endif