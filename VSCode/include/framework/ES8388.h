#pragma once
#ifndef ES8388_H
#define ES8388_H

#include "Bitwise.h"
#include "Debug.h"
#include "ESPDebug.h"
#include "I2CUtils.h"
#include "ES8388Interface.h"
#include <driver/i2s.h>
#include <driver/i2c.h>

class ES8388
{
public:
	enum class Modules
	{
		ADC = 0x1,
		DAC = 0x2,
		Both = ADC | DAC,
		Line = 0x4
	};

	enum class BitsPerSamples
	{
		None = -1,
		BPS16 = 0x03,
		BPS24 = 0x00,
		BPS32 = 0x04,
	};

	enum class InputModes
	{
		None = -1,
		LeftAndRightInput1 = 0x00,
		Microphone1 = 0x05,
		Microphone2 = 0x06,
		LeftAndRightInput2 = 0x50,
		Difference = 0xf0
	};

	enum class OutputModes
	{
		None = -1,
		LeftOutput1 = 0x20,
		LeftOutput2 = 0x08,
		SpeakerOutput = 0x09,
		RightOutput1 = 0x10,
		RightOutput2 = 0x04,
		AllLineOutputs = LeftOutput1 | LeftOutput2 | RightOutput1 | RightOutput2
	};

	enum class Formats
	{
		Normal = 0,
		Left,
		Right,
		DSP,
	};

public:
	ES8388(Modules Module, BitsPerSamples BitsPerSample, InputModes InputMode, OutputModes OutputMode, Formats Format)
		: m_InputMode(InputMode)
	{
		Log::WriteInfo(TAG, "Intializing");

		CHECK_CALL(ES8388Interface::TurnOn(false, ES8388Interface::MiddleVoltageResistances::R500K));

		CHECK_CALL(ES8388Interface::SetDACPowered(true, ES8388Interface::IOModes::All, ES8388Interface::OutputResistances::R1K5));
		CHECK_CALL(ES8388Interface::SetADCPowered(true, false, ES8388Interface::IOModes::All));

		CHECK_CALL(ES8388Interface::SetADCBitsPerSample(ES8388Interface::BitsPerSamples::BPS16));
		CHECK_CALL(ES8388Interface::SetDACBitsPerSample(ES8388Interface::BitsPerSamples::BPS16));

		CHECK_CALL(ES8388Interface::SetADCFormat(ES8388Interface::Formats::I2S));
		CHECK_CALL(ES8388Interface::SetDACFormat(ES8388Interface::Formats::I2S));

		CHECK_CALL(ES8388Interface::SetInputToMixerGain(6));

		CHECK_CALL(ES8388Interface::SetDigitalVolume(0));

		CHECK_CALL(ES8388Interface::SetMicrophoneGain(24));

		CHECK_CALL(ES8388Interface::SetInputVolume(0));

		CHECK_CALL(ES8388Interface::SetOutputVolume(4.5F));

		CHECK_CALL(ES8388Interface::SetOutputMute(false));
	}

private:
	InputModes m_InputMode;

	static const char *TAG;
};

const char *ES8388::TAG = "ES8388";

#endif