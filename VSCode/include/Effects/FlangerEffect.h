#ifdef FLANGER_EFFECT

#pragma once
#ifndef FLANGER_EFFECT_H
#define FLANGER_EFFECT_H

#include "Effect.h"
#include <framework/include/DSP/DSPs/Flanger.h>

template <typename T>
class FlangerEffect : public Effect<T>
{
public:
	FlangerEffect(ControlManager *ControlManager, uint32 SampleRate)
		: Effect<T>(ControlManager, GPIOPins::Pin22, GPIOPins::Pin19),
		  m_Flanger(SampleRate),
		  m_FeedbackPot(nullptr),
		  m_WetRatePot(nullptr),
		  m_DepthPot(nullptr),
		  m_RatePot(nullptr)
	{
		m_FeedbackPot = ControlManager->CreatePotentiometer("Feedback", GPIOPins::Pin4);
		m_FeedbackPot->SetOnChangedListener(
			[&](float value)
			{
				m_Flanger.SetFeedback(value);
			});

		m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
		m_WetRatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Flanger.SetWetRate(value);
			});

		m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin14);
		m_DepthPot->SetOnChangedListener(
			[&](float value)
			{
				m_Flanger.SetDepth(Math::Lerp(0.0, Flanger<T>::MAX_DEPTH, value));
			});

		m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
		m_RatePot->SetOnChangedListener(
			[&](float value)
			{
				m_Flanger.SetRate(Math::Lerp(0.01, 4, value));
			});
	}

protected:
	IDSP<T> *GetDSP(void)
	{
		return &m_Flanger;
	}

private:
	Flanger<T> m_Flanger;
	Potentiometer *m_FeedbackPot;
	Potentiometer *m_WetRatePot;
	Potentiometer *m_DepthPot;
	Potentiometer *m_RatePot;
};

#endif

#endif