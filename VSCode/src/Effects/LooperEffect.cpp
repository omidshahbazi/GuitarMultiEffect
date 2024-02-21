#ifdef LOOPER_EFFECT

#include "Effects/LooperEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Switch.h>
#include <framework/include/Controls/Potentiometer.h>

LooperEffect::LooperEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_Looper(SampleRate),
	  m_ModeSwitch(nullptr),
	  m_VolumePot(nullptr)
{
	m_ModeSwitch = ControlManager->CreateSwitch("Mode", GPIOPins::Pin21);
	m_ModeSwitch->SetOnChangedListener(
		[&](bool value) {

		});

	m_VolumePot = ControlManager->CreatePotentiometer("Volume", GPIOPins::Pin15);
	m_VolumePot->SetOnChangedListener(
		[&](float value)
		{
			m_Looper.SetVolume(value);
		});
}

IDSP *LooperEffect::GetDSP(void)
{
	return &m_Looper;
}

#endif