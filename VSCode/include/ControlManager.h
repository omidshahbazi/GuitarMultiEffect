#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "Common.h"
#include "framework/DSP/Controls/ControlFactory.h"
#include "framework/ILI9341_HAL.h"

class ControlManager
{
public:
	ControlManager(IHAL *HAL)
		: m_Factory(HAL),
#ifdef DEBUG
		  m_BootButton(nullptr),
#endif
		  m_VolumePotentiometer(nullptr),
		  m_Display(HAL, GPIOPins::Pin8, GPIOPins::Pin10, GPIOPins::Pin7, GPIOPins::Pin6, GPIOPins::Pin9, I_LCD_HAL::Orientations::ToLeft)
	{
	}

	void Initialize(void)
	{
#ifdef DEBUG
		m_BootButton = CreateButton("Boot Mode Button", GPIOPins::Pin30);
#endif

		m_VolumePotentiometer = CreatePotentiometer("Volume", AnalogPins ::Pin2);

		m_Display.Initialize();
		m_Display.SetTargetFrameRate(25);
	}

#ifdef DEBUG
	void SetBootButtonCallback(Button::TurnedOffEventHandler Listener)
	{
		m_BootButton->SetOnTurnedOffListener(Listener);
	}
#endif

	void SetVolumeCallback(Potentiometer::EventHandler Listener)
	{
		m_VolumePotentiometer->SetOnChangedListener(Listener);
	}

	void SetDisplayallback(ILI9341_HAL_320_240::RenderEventHandler Listener)
	{
		m_Display.SetOnRender(Listener);
		m_Display.SetTargetFrameRate(24);
	}

	void Update(void)
	{
		m_Factory.Process();

		m_Display.Update();
	}

private:
	DualLED *CreateDualLED(cstr Name, GPIOPins RedPin, GPIOPins GreenPin)
	{
		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);

		return m_Factory.CreateDualLED((uint8)RedPin, (uint8)GreenPin);
	}

	Button *CreateButton(cstr Name, GPIOPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Button %i", Name, Pin);

		return m_Factory.CreateButton((uint8)Pin);
	}

	Potentiometer *CreatePotentiometer(cstr Name, AnalogPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

		Potentiometer *pot = m_Factory.CreatePotentiometer((uint8)Pin);
		pot->SetCalibrationValues(0.01, 0.99);

		return pot;
	}

	RotaryButton *CreateRotaryButton(cstr Name, GPIOPins LeftPin, GPIOPins RightPin, GPIOPins ButtonPin)
	{
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, LeftPin);
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, RightPin);
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, ButtonPin);

		return m_Factory.CreateRotaryButton((uint8)LeftPin, (uint8)RightPin, (uint8)ButtonPin);
	}

public:
	ILI9341_HAL_320_240 *GetDisplay(void)
	{
		return &m_Display;
	}

private:
	ControlFactory<(uint8)GPIOPins::COUNT, 100> m_Factory;

#ifdef DEBUG
	Button *m_BootButton;
#endif
	Potentiometer *m_VolumePotentiometer;
	ILI9341_HAL_320_240 m_Display;
};

#endif