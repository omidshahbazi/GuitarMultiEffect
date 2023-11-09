#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "Effect.h"
#include "framework/include/Math.h"
#include "framework/include/Wave/ToneControl.h"

class OverdriveEffect : public Effect
{
public:
	OverdriveEffect(void)
		: m_Drive(0.5)
	{
	}

	void SetDrive(float Value)
	{
		Value = Math::Clamp01(Value);

		m_Drive = Value;
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

protected:
	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		// for (uint16 i = 0; i < Count; ++i)
		// {
		// 	Buffer[i] = tanh(Buffer[i] * m_Drive);
		// }
		for (uint16 i = 0; i < Count; ++i)
		{
			Buffer[i] = m_ToneControl.Process(Buffer[i]);
		}
	}

private:
	float m_Drive;

	ToneControl m_ToneControl;
};

#endif