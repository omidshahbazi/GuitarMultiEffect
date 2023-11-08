#pragma once
#ifndef OVERDRIVE_EFFECT_H
#define OVERDRIVE_EFFECT_H

#include "IEffect.h"
#include "framework/include/Math.h"

class OverdriveEffect : public IEffect
{
public:
	OverdriveEffect(void)
		: m_Drive(1)
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

	void Process(float *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = tanh(Buffer[i] * m_Drive);
	}

private:
	float m_Drive;
};

#endif