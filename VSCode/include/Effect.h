#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "framework/include/Common.h"

class Effect
{
public:
	Effect(void)
		: m_Enabled(true)
	{
	}

	void Apply(double *Buffer, uint16 Count)
	{
		if (!m_Enabled)
			return;

		ProcessBuffer(Buffer, Count);
	}

	virtual void ToggleEnabled(void)
	{
		m_Enabled = !m_Enabled;
	}

protected:
	virtual void ProcessBuffer(double *Buffer, uint16 Count) = 0;

private:
	bool m_Enabled;
};

#endif