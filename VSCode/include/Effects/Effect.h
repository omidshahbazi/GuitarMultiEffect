#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include <framework/include/Common.h>

class IDSP;

class Effect
{
public:
	Effect(void);

	void Apply(double *Buffer, uint16 Count);

	virtual void ToggleEnabled(void);

protected:
	virtual IDSP *GetDSP(void) = 0;

	virtual void ProcessBuffer(double *Buffer, uint16 Count);

private:
	bool m_Enabled;
};

#endif