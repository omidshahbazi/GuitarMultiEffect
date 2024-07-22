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
		  m_SaveButton(nullptr),
		  m_BackButton(nullptr),
		  m_ValueControl(nullptr),
		  m_VolumePotentiometer(nullptr),
		  m_UpButton(nullptr),
		  m_DownButton(nullptr),
		  m_Variation1LED(nullptr),
		  m_Variation1Button(nullptr)
	{
	}

	void Initialize(void)
	{
		m_Display.Initialize();
		m_Display.SetTargetFrameRate(24);

		m_BootButton = CreateButton("Boot Mode", GPIOPins::Pin3);

		m_SaveButton = CreateButton("Save", GPIOPins::Pin4);

		m_BackButton = CreateButton("Back", GPIOPins::Pin5);

		m_ValueControl = CreateRotaryButton("Value", GPIOPins::Pin1, GPIOPins::Pin2, GPIOPins::Pin0);

		m_VolumePotentiometer = CreatePotentiometer("Master Volume", AnalogPins ::Pin2);

		m_UpButton = CreateButton("Up", GPIOPins::Pin26);
		m_DownButton = CreateButton("Down", GPIOPins::Pin27);

		m_Variation1LED = CreateDualLED("Variation 1", GPIOPins::Pin11, GPIOPins::Pin12);
		m_Variation1Button = CreateButton("Variation 1", GPIOPins::Pin28);

		m_Variation2LED = CreateDualLED("Variation 2", GPIOPins::Pin13, GPIOPins::Pin14);
		m_Variation2Button = CreateButton("Variation 2", GPIOPins::Pin29);

		m_Variation3LED = CreateDualLED("Variation 3", GPIOPins::Pin24, GPIOPins::Pin25);
		m_Variation3Button = CreateButton("Variation 3", GPIOPins::Pin30);
	}

	void SetDisplayCallback(ILI9341_HAL_320_240::RenderEventHandler Listener)
	{
		m_Display.SetOnRender(Listener);
	}

	void SetBootButtonCallback(Button::TurnedOffEventHandler Listener)
	{
		m_BootButton->SetOnTurnedOffListener(Listener);
	}

	void SetSaveButtonCallback(Button::TurnedOffEventHandler Listener)
	{
		m_SaveButton->SetOnTurnedOffListener(Listener);
	}

	void SetBackButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_BackButton->SetOnTurnedOffListener(Listener);
	}

	void SetValueRotatedCallback(RotaryButton::RotatedEventHandler Listener)
	{
		m_ValueControl->SetOnRotatedListener(Listener);
	}

	void SetValueButtonHoldCallback(Button::HoldEventHandler Listener)
	{
		m_ValueControl->SetOnHoldListener(Listener);
	}

	void SetValueButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_ValueControl->SetOnTurnedOffListener(Listener);
	}

	void SetVolumeCallback(Potentiometer::EventHandler Listener)
	{
		m_VolumePotentiometer->SetOnChangedListener(Listener);
	}

	void SetUpButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_UpButton->SetOnTurnedOffListener(Listener);
	}

	void SetDownButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_DownButton->SetOnTurnedOffListener(Listener);
	}

	void SetVariation1LEDConstantBrightness(Color Color)
	{
		m_Variation1LED->SetConstantBrighness(1);
		m_Variation1LED->SetColor(Color);
	}

	void SetVariation1ButtonTurnedOnCallback(Button::TurnedOnEventHandler Listener)
	{
		m_Variation1Button->SetOnTurnedOnListener(Listener);
	}

	void SetVariation1ButtonHoldCallback(Button::HoldEventHandler Listener)
	{
		m_Variation1Button->SetOnHoldListener(Listener);
	}

	void SetVariation1ButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_Variation1Button->SetOnTurnedOffListener(Listener);
	}

	void SetVariation2LEDConstantBrightness(Color Color)
	{
		m_Variation2LED->SetConstantBrighness(1);
		m_Variation2LED->SetColor(Color);
	}

	void SetVariation2ButtonTurnedOnCallback(Button::TurnedOnEventHandler Listener)
	{
		m_Variation2Button->SetOnTurnedOnListener(Listener);
	}

	void SetVariation2ButtonHoldCallback(Button::HoldEventHandler Listener)
	{
		m_Variation2Button->SetOnHoldListener(Listener);
	}

	void SetVariation2ButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_Variation2Button->SetOnTurnedOffListener(Listener);
	}

	void SetVariation3LEDConstantBrightness(Color Color)
	{
		m_Variation3LED->SetConstantBrighness(1);
		m_Variation3LED->SetColor(Color);
	}

	void SetVariation3ButtonTurnedOnCallback(Button::TurnedOnEventHandler Listener)
	{
		m_Variation3Button->SetOnTurnedOnListener(Listener);
	}

	void SetVariation3ButtonHoldCallback(Button::HoldEventHandler Listener)
	{
		m_Variation3Button->SetOnHoldListener(Listener);
	}

	void SetVariation3ButtonTurnedOffCallback(Button::TurnedOffEventHandler Listener)
	{
		m_Variation3Button->SetOnTurnedOffListener(Listener);
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
	Button *m_SaveButton;
	Button *m_BackButton;
	RotaryButton *m_ValueControl;
	Potentiometer *m_VolumePotentiometer;
	Button *m_UpButton;
	Button *m_DownButton;

	DualLED *m_Variation1LED;
	Button *m_Variation1Button;

	DualLED *m_Variation2LED;
	Button *m_Variation2Button;

	DualLED *m_Variation3LED;
	Button *m_Variation3Button;
};

#endif