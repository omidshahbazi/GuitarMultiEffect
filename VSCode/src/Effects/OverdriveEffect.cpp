// #ifdef OVERDRIVE_EFFECT

// #include "Effects/OverdriveEffect.h"
// #include "ControlManager.h"
// #include <framework/include/Controls/Potentiometer.h>

// template <typename T>
// OverdriveEffect<T>::OverdriveEffect(ControlManager *ControlManager, uint32 SampleRate)
// 	: Effect<T>(ControlManager),
// 	  m_GainPot(nullptr),
// 	  m_DrivePot(nullptr)
// {
// 	m_GainPot = ControlManager->CreatePotentiometer("Gain", GPIOPins::Pin14);
// 	m_GainPot->SetOnChangedListener(
// 		[&](float value)
// 		{
// 			m_Overdrive.SetGain(value);
// 		});

// 	m_DrivePot = ControlManager->CreatePotentiometer("Drive", GPIOPins::Pin15);
// 	m_DrivePot->SetOnChangedListener(
// 		[&](float value)
// 		{
// 			m_Overdrive.SetDrive(value);
// 		});
// }

// #endif