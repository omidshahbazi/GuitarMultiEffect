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
		  m_Display(HAL, GPIOPins::Pin8, GPIOPins::Pin10, GPIOPins::Pin7, GPIOPins::Pin6, GPIOPins::Pin9, I_LCD_HAL::Orientations::ToRight),
		  m_BootButton(nullptr),
		  m_SaveButon(nullptr),
		  m_BackButon(nullptr),
		  m_ValueControl(nullptr),
		  m_VolumePotentiometer(nullptr),
		  m_UpButton(nullptr),
		  m_DownButon(nullptr),
		  m_LooperLED(nullptr),
		  m_LooperButon(nullptr)
	{
	}

	void Initialize(void)
	{
		m_Display.Initialize();
		m_Display.SetTargetFrameRate(24);

		m_BootButton = CreateButton("Boot Mode", GPIOPins::Pin3);

		m_SaveButon = CreateButton("Save", GPIOPins::Pin4);

		m_BackButon = CreateButton("Back", GPIOPins::Pin5);

		m_ValueControl = CreateRotaryButton("Value", GPIOPins::Pin1, GPIOPins::Pin2, GPIOPins::Pin0);

		m_VolumePotentiometer = CreatePotentiometer("Master Volume", AnalogPins ::Pin2);

		m_UpButton = CreateButton("Up", GPIOPins::Pin26);
		m_DownButon = CreateButton("Down", GPIOPins::Pin27);

		m_LooperLED = CreateDualLED("Looper", GPIOPins::Pin24, GPIOPins::Pin25);
		m_LooperButon = CreateButton("Looper", GPIOPins::Pin30);
	}

	void SetDisplayCallback(ILI9341_HAL_320_240::RenderEventHandler Listener)
	{
		m_Display.SetOnRender(Listener);
	}

	void SetBootButtonCallback(Button::TurnedOffEventHandler Listener)
	{
		m_BootButton->SetOnTurnedOffListener(Listener);
	}

	void SetSaveButonCallback(Button::TurnedOffEventHandler Listener)
	{
		m_SaveButon->SetOnTurnedOffListener(Listener);
	}

	void SetBackButtonTunedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_BackButon->SetOnTurnedOffListener(Listener);
	}

	void SetValueRotatedCallback(RotaryButton::RotatedEventHandler Listener)
	{
		m_ValueControl->SetOnRotatedListener(Listener);
	}

	void SetValueButtonHoldCallback(Button::HoldEventHandler Listener)
	{
		m_ValueControl->SetOnHoldListener(Listener);
	}

	void SetValueButtonTunedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_ValueControl->SetOnTurnedOffListener(Listener);
	}

	void SetVolumeCallback(Potentiometer::EventHandler Listener)
	{
		m_VolumePotentiometer->SetOnChangedListener(Listener);
	}

	void SetUpButtonTunedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_UpButton->SetOnTurnedOffListener(Listener);
	}

	void SetDownButtonTunedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_DownButon->SetOnTurnedOffListener(Listener);
	}

	void SetLooperLEDConstantBrightness(Color Color)
	{
		m_LooperLED->SetConstantBrighness(1);
		m_LooperLED->SetColor(Color);
	}

	void SetLooperButtonTunedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_LooperButon->SetOnTurnedOffListener(Listener);
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
		pot->SetCalibrationValues(0.001, 0.96);

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

	ILI9341_HAL_320_240 m_Display;

	Button *m_BootButton;
	Button *m_SaveButon;
	Button *m_BackButon;
	RotaryButton *m_ValueControl;
	Potentiometer *m_VolumePotentiometer;
	Button *m_UpButton;
	Button *m_DownButon;
	DualLED *m_LooperLED;
	Button *m_LooperButon;
};

#endif