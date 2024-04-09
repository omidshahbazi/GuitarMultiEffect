// #ifdef PHASER_EFFECT

// #include "Effects/PhaserEffect.h"
// #include "ControlManager.h"
// #include <framework/include/Controls/Potentiometer.h>

// PhaserEffect::PhaserEffect(ControlManager *ControlManager, uint32 SampleRate)
// 	: Effect(ControlManager),
// 	  m_Phaser(SampleRate),
// 	  m_WetRatePot(nullptr),
// 	  m_DepthPot(nullptr),
// 	  m_RatePot(nullptr)
// {
// 	m_WetRatePot = ControlManager->CreatePotentiometer("Wet Rate", GPIOPins::Pin13);
// 	m_WetRatePot->SetOnChangedListener(
// 		[&](float value)
// 		{
// 			m_Phaser.SetWetRate(value);
// 		});

// 	m_DepthPot = ControlManager->CreatePotentiometer("Depth", GPIOPins::Pin14);
// 	m_DepthPot->SetOnChangedListener(
// 		[&](float value)
// 		{
// 			m_Phaser.SetDepth(Math::Lerp(0.0, Phaser::MAX_DEPTH, value));
// 		});

// 	m_RatePot = ControlManager->CreatePotentiometer("Rate", GPIOPins::Pin15);
// 	m_RatePot->SetOnChangedListener(
// 		[&](float value)
// 		{
// 			m_Phaser.SetRate(Math::Lerp(0.1, 3.7, value));
// 		});
// }

// IDSP *PhaserEffect::GetDSP(void)
// {
// 	return &m_Phaser;
// }

// #endif