#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "../ControlManager.h"
#include "../framework/Common.h"
#include "../framework/DSP/DSPs/IDSP.h"

template <typename T>
class Effect
{
public:
	Effect(ControlManager *ControlManager, GPIOPins RedLEDPin, GPIOPins GreenLEDPin, GPIOPins EnableButtonPin)
		: m_Enabled(true)
	{
		m_EnabledLED = ControlManager->CreateDualLED("Enabled", RedLEDPin, GreenLEDPin);
		m_EnabledLED->SetColor(DUAL_LED_RED);
		m_EnabledLED->SetConstantBrighness(1);

		auto onSwitchChanged = [&](bool value)
		{
			m_Enabled = value;

			if (m_Enabled)
				m_EnabledLED->SetBlinkingBrighness(1, 1);
			else
				m_EnabledLED->SetConstantBrighness(0);
		};

		m_EnabledSwitch = ControlManager->CreateSwitch("Enabled", EnableButtonPin);
		m_EnabledSwitch->SetOnStateChangedListener(onSwitchChanged);

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
	DualLED *m_EnabledLED;
	Switch *m_EnabledSwitch;
};

#endif