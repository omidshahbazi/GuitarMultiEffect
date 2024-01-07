#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include <framework/include/Common.h>

class IDSP;
class ControlManager;
class Switch;
class LED;

class Effect
{
public:
	Effect(ControlManager *ControlManager);

	void Apply(double *Buffer, uint16 Count);

protected:
	virtual IDSP *GetDSP(void) = 0;

private:
	bool m_Enabled;
	Switch *m_EnabledSwitch;
	LED *m_EnabledLED;
};

#endif