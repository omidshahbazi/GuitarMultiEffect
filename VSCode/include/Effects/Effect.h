#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "../ControlManager.h"
#include "../framework/Common.h"
#include "../framework/DSP/DSPs/IDSP.h"
#include "../framework/DSP/Controls/Switch.h"
#include "../framework/DSP/Controls/SingleLED.h"
#include "../framework/DSP/Controls/Potentiometer.h"

template <typename T>
class Effect
{
public:
	Effect(ControlManager *ControlManager)
		: m_Enabled(true)
	{
		// m_EnabledLED = ControlManager->CreateSingleLED("Enabled", GPIOPins::Pin22);
		// m_EnabledLED->SetConstantBrighness(0);

		// auto onSwitchChanged = [&](bool value)
		// {
		// 	m_Enabled = value;

		// 	if (m_Enabled)
		// 		m_EnabledLED->SetBlinkingBrighness(1, 1);
		// 	else
		// 		m_EnabledLED->SetConstantBrighness(0);
		// };

		// m_EnabledSwitch = ControlManager->CreateSwitch("Enabled", GPIOPins::Pin19);
		// m_EnabledSwitch->SetOnStateChangedListener(onSwitchChanged);

		// onSwitchChanged(m_EnabledSwitch->GetTurnedOn());
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
	SingleLED *m_EnabledLED;
};

#endif