#ifdef SUSTAIN_EFFECT

#pragma once
#ifndef SUSTAIN_EFFECT_H
#define SUSTAIN_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Sustain.h>

template <typename T>
class SustainEffect : public Effect<T>
{
public:
	SustainEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager),
		  m_Sustain(SampleRate, MAX_DELAY_TIME),
		  m_FeedbackPot(nullptr),
		  m_ActiveSwitch(nullptr),
		  m_WetSwitch(nullptr)
	{
		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", GPIOPins::Pin15);
		m_FeedbackPot->SetOnChangedListener(
			[&](float value)
			{
				m_Sustain.SetFeedback(value);
			});

		m_ActiveSwitch = ControlManager->CreateSwitch("Active", GPIOPins::Pin21);
		m_ActiveSwitch->SetOnStateChangedListener(
			[&](bool value)
			{
				m_Sustain.SetActive(value);
			});

		m_WetSwitch = ControlManager->CreateSwitch("Wet", GPIOPins::Pin22);
		m_WetSwitch->SetOnStateChangedListener(
			[&](bool value)
			{
				m_Sustain.SetWet(value);
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Sustain;
	}

private:
	Sustain<T> m_Sustain;
	Potentiometer *m_FeedbackPot;
	Switch *m_ActiveSwitch;
	Switch *m_WetSwitch;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif

#endif