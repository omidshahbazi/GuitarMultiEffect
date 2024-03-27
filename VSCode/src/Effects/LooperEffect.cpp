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
	// TODO: this needs to set different modes using a single switch or probably two?

	m_ModeSwitch = ControlManager->CreateSwitch("Mode", GPIOPins::Pin21);
	m_ModeSwitch->SetOnChangedListener(
		[&](bool value)
		{
			m_Looper.SetMode(value ? Looper::Modes::Record : Looper::Modes::Replay);
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