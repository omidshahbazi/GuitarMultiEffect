#include "Effects/Effect.h"
#include "ControlManager.h"
#include <framework/include/DSP/IDSP.h>
#include <framework/include/Controls/Switch.h>
#include <framework/include/Controls/LED.h>

Effect::Effect(ControlManager *ControlManager)
	: m_Enabled(true)
{
	m_EnabledLED = ControlManager->CreateLED(GPIOPins::Pin23);

	m_EnabledSwitch = ControlManager->CreateSwitch(GPIOPins::Pin19);
	m_EnabledSwitch->SetOnChangedListener(
		[&](bool value)
		{
			m_Enabled = value;

			if (m_Enabled)
				m_EnabledLED->SetBlinking(2);
			else
				m_EnabledLED->SetTurnedOn(false);
		});
}

void Effect::Apply(double *Buffer, uint16 Count)
{
	if (!m_Enabled)
		return;

	GetDSP()->ProcessBuffer(Buffer, Count);
}