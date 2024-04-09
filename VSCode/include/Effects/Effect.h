#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "ControlManager.h"
#include <framework/include/Common.h>
#include <framework/include/DSP/DSPs/IDSP.h>
#include <framework/include/Controls/Switch.h>
#include <framework/include/Controls/LED.h>
#include <framework/include/Controls/Potentiometer.h>

template <typename T>
class Effect
{
public:
	Effect(ControlManager *ControlManager)
		: m_Enabled(true)
	{
		m_EnabledLED = ControlManager->CreateLED("Enabled", GPIOPins::Pin23);
		m_EnabledLED->SetTurnedOn(false);

		auto onSwitchChanged = [&](bool value)
		{
			m_Enabled = value;

			if (m_Enabled)
				m_EnabledLED->SetBlinking(2);
			else
				m_EnabledLED->SetTurnedOn(false);
		};

		m_EnabledSwitch = ControlManager->CreateSwitch("Enabled", GPIOPins::Pin19);
		m_EnabledSwitch->SetOnChangedListener(onSwitchChanged);

		onSwitchChanged(m_EnabledSwitch->GetTurnedOn());
	}

	void Apply(T *Buffer, uint16 Count)
	{
		if (!m_Enabled)
			return;

		GetDSP()->ProcessBuffer(Buffer, Count);
	}

protected:
	virtual IDSP<T> *GetDSP(void) = 0;

private:
	bool m_Enabled;
	Switch *m_EnabledSwitch;
	LED *m_EnabledLED;
};

#endif