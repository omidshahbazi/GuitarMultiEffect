#ifdef FLANGER_EFFECT

#include "Effects/FlangerEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>

FlangerEffect::FlangerEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
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
			m_Flanger.SetDepth(Math::Lerp(0.0, Flanger::MAX_DEPTH, value));
		});

	m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
	m_RatePot->SetOnChangedListener(
		[&](float value)
		{
			m_Flanger.SetRate(Math::Lerp(0.01, 4, value));
		});
}

IDSP *FlangerEffect::GetDSP(void)
{
	return &m_Flanger;
}

#endif