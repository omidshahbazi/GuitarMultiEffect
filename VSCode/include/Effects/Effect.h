#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "ControlManager.h"
#include <framework/include/Common.h>
#include <framework/include/DSP/DSPs/IDSP.h>
#include <framework/include/DSP/Controls/Switch.h>
#include <framework/include/DSP/Controls/SingleLED.h>
#include <framework/include/DSP/Controls/Potentiometer.h>

template <typename T>
class Effect
{
public:
	Effect(ControlManager *ControlManager, GPIOPins LEDPin, GPIOPins EnableButtonPin)
		: m_Enabled(true)
	{
		m_EnabledLED = ControlManager->CreateSingleLED("Enabled", LEDPin);

		m_EnabledSwitch = ControlManager->CreateSwitch("Enabled", EnableButtonPin);
		m_EnabledSwitch->SetOnTurnedOnListener({this, [](void *Context)
												{ static_cast<Effect *>(Context)->OnTurnedOn(); }});

		m_EnabledSwitch->SetOnTurnedOffListener({this, [](void *Context, float HeldTime)
												 { static_cast<Effect *>(Context)->OnTurnedOff(HeldTime); }});

		UpdateEnabledState();
	}

	void Apply(T *Buffer, uint16 Count)
	{
		if (!m_Enabled)
			return;

		GetDSP()->ProcessBuffer(Buffer, Count);
	}

protected:
	void SetLEDBlinkingEnabled(bool Enabled, float Rate = 1)
	{
		if (m_Enabled && Enabled)
			m_EnabledLED->SetBlinkingBrighness(1, Rate);
		else
			UpdateEnabledState();
	}

	virtual void SetEnabled(bool Value)
	{
		m_Enabled = Value;

		m_EnabledLED->SetConstantBrighness(m_Enabled);
	}

	virtual void UpdateEnabledState(void)
	{
		SetEnabled(GetIsEnabledButtonOn());
	}

	virtual void OnTurnedOn(void)
	{
		UpdateEnabledState();
	}

	virtual void OnTurnedOff(float TurnedOnTime)
	{
		UpdateEnabledState();
	}

	virtual bool GetIsEnabledButtonOn(void) const
	{
		return m_EnabledSwitch->GetTurnedOn();
	}

	virtual IDSP<T> *GetDSP(void) = 0;

private:
	bool m_Enabled;
	Switch *m_EnabledSwitch;
	SingleLED *m_EnabledLED;
};

#endif