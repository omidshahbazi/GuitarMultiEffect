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

private:
	enum class ChipRegisters
	{
		Power = 0x02,
		Mode = 0x08,
		Control1 = 0x00,
		Control2 = 0x01
	};

	enum class ADCRegisters
	{
		Power = 0x03,
		Control1 = 0x09,
		Control2 = 0x0a,
		Control3 = 0x0b,
		Control4 = 0x0c,
		Control5 = 0x0d,
		Control6 = 0x0e,
		Control7 = 0x0f,
		Control8 = 0x10,
		Control9 = 0x11,
		Control10 = 0x12,
		Control11 = 0x13,
		Control12 = 0x14,
		Control13 = 0x15,
		Control14 = 0x16
	};

	enum class DACRegisters
	{
		Power = 0x04,
		Control1 = 0x17,
		Control2 = 0x18,
		Control3 = 0x19,
		Control4 = 0x1a,
		Control5 = 0x1b,
		Control6 = 0x1c,
		Control7 = 0x1d,
		Control8 = 0x1e,
		Control9 = 0x1f,
		Control10 = 0x20,
		Control11 = 0x21,
		Control12 = 0x22,
		Control13 = 0x23,
		Control14 = 0x24,
		Control15 = 0x25,
		Control16 = 0x26,
		Control17 = 0x27,
		Control18 = 0x28,
		Control19 = 0x29,
		Control20 = 0x2a,
		Control21 = 0x2b,
		Control22 = 0x2c,
		Control23 = 0x2d,
		Control24 = 0x2e,
		Control25 = 0x2f,
		Control26 = 0x30,
		Control27 = 0x31,
		Control28 = 0x32,
		Control29 = 0x33,
		Control30 = 0x34
	};

public:
	ES8388(Modules Module, BitsPerSamples BitsPerSample, InputModes InputMode, OutputModes OutputMode, Formats Format)
		: m_InputMode(InputMode)
	{
		Log::WriteInfo(TAG, "Intializing");

#if 1
		ES8388Interface::Initialize();
#else
		CHECK_CALL(SetRegisters(InputMode, OutputMode));

		CHECK_CALL(ConfigI2S(BitsPerSample, Format));

		CHECK_CALL(SetOutputVolume(70));

		CHECK_CALL(Start(Module));
#endif
	}

	// Get and set the output level (analog gain)
	// vol = 0-30 for ES83-version module
	// vol = 0-31 for AC101-version module
	bool SetOutputVolume(uint8 Value)
	{
		if (Value < 0)
			Value = 0;
		else if (Value > 100)
			Value = 100;

		Value /= 3;

		Log::WriteInfo(TAG, "Setting the volume: %i", Value);

		I2CWrite(DACRegisters::Control24, Value); // LOUT1VOL
		I2CWrite(DACRegisters::Control25, Value); // ROUT1VOL
		I2CWrite(DACRegisters::Control26, Value); // LOUT2VOL
		I2CWrite(DACRegisters::Control27, Value); // ROUT2VOL

		return true;
	}
	uint8 GetOutputVolume(void)
	{
		return I2CRead(DACRegisters::Control24);
	}

	bool SetMute(bool Enabled)
	{
		Log::WriteInfo(TAG, "Setting the Mute: %i", Enabled);

		I2CWrite(DACRegisters::Control3, (I2CRead(DACRegisters::Control3) & 0xFB) | (((int32)Enabled) << 2));

		return true;
	}

	// Get and set the input gain (analog)
	bool SetInputGain(uint8 Value)
	{
		if (Value > 8)
			Value = 8;

		Log::WriteInfo(TAG, "Setting the Input Gain: %i", Value);

		uint8 temp = I2CRead(ADCRegisters::Control1);

		if (m_InputMode == InputModes::LeftAndRightInput1) // input mode = IM_LR
			temp = (Value << 4) | Value;
		else // input mode = IM_LMIC
			temp = (0x0F & temp) | (Value << 4);

		I2CWrite(ADCRegisters::Control1, temp);

		return true;
	}
	uint8 GetInputGain(void)
	{
		return (I2CRead(ADCRegisters::Control1) & 0xF0) >> 4;
	}

	// Get and set microphone gain (0:0dB,1-7:30dB-48dB)
	bool SetMicrophoneGain(uint8 Value)
	{
		// if (m_InputMode != InputModes::Microphone1 && m_InputMode != InputModes::Microphone2)
		// 	return false;
		// input mode = LMIC/RMIC

		if (Value > 8)
			Value = 8;

		Log::WriteInfo(TAG, "Setting the Microphone Gain: %i", Value);

		I2CWrite(ADCRegisters::Control1, (I2CRead(ADCRegisters::Control1) & 0xF0) | Value);

		return true;
	}
	uint8 GetMicrophoneGain(void)
	{
		if (m_InputMode != InputModes::Microphone1 && m_InputMode != InputModes::Microphone2)
			return false;
		// input mode = LMIC/RMIC

		return (0x0F & I2CRead(ADCRegisters::Control1));
	}

	// Get and set microphone noise gate (0-31: -76.5dB, -75.0dB,...., -30.0dB)
	bool SetMicrophoneNoiseGate(uint8 Value)
	{
		if (m_InputMode != InputModes::Microphone1 && m_InputMode != InputModes::Microphone2)
			return false;
		// input mode = LMIC/RMIC

		if (Value > 32)
			Value = 32;

		Log::WriteInfo(TAG, "Setting the Microphone Noise Gate: %i", Value);

		if (Value > 0)
			I2CWrite(ADCRegisters::Control14, ((Value - 1) << 3) | 1);
		else // turn off the noise gate at gate = 0
			I2CWrite(ADCRegisters::Control14, 0);

		return true;
	}
	uint8 GetMicrophoneNoiseGate(void)
	{
		if (m_InputMode != InputModes::Microphone1 && m_InputMode != InputModes::Microphone2)
			return false;
		// input mode = LMIC/RMIC

		return I2CRead(ADCRegisters::Control14) >> 3;
	}

	// Optimize the analog to digital conversion range
	// range: 0, 1, 2, 3, 4 (1Vrms/2.83Vpp, 0.5Vrms/1.41Vpp, 0.25Vrms/707mVpp, 0.125Vrms/354mVpp, 0.625Vrms/177mVpp)
	void OptimizeConversion(uint8 Range = 2)
	{
		static uint8 INPUT_GAIN[] = {0, 2, 4, 6, 8};		 // 0db, 6dB, 12dB, 18dB, 24dB
		static uint8 OUTPUT_VOLUME[] = {30, 26, 22, 18, 14}; // 0db, -6dB, -12dB, -18dB, -24dB

		if (Range < 0)
			Range = 0;
		if (Range > 4)
			Range = 4;

		Log::WriteInfo(TAG, "Optimizing Conversion: %i, InputGain: %i, Output Volume: %i", Range, INPUT_GAIN[Range], OUTPUT_VOLUME[Range]);

		SetInputGain(INPUT_GAIN[Range]);
		SetOutputVolume(OUTPUT_VOLUME[Range]);
	}

private:
	static bool SetRegisters(InputModes InputMode, OutputModes OutputMode)
	{
		Log::WriteInfo(TAG, "Setting the Registers");

		enum class MasterModes
		{
			None = -1,
			Slave = 0x00,
			Master = 0x01
		};

		I2CWrite(DACRegisters::Control3, 0x04); // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp

		I2CWrite(ChipRegisters::Control2, 0x50);
		I2CWrite(ChipRegisters::Power, 0x00);					  // normal all and power up all
		I2CWrite(ChipRegisters::Mode, (uint8)MasterModes::Slave); // CODEC IN I2S SLAVE MODE

		I2CWrite(DACRegisters::Power, 0xC0);	 // disable DAC and disable Lout/Rout/1/2
		I2CWrite(ChipRegisters::Control1, 0x12); // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		I2CWrite(DACRegisters::Control1, 0x18);	 // 1a 0x18:16bit iis , 0x00:24
		I2CWrite(DACRegisters::Control2, 0x02);	 // DACFsMode,SINGLE SPEED; DACFsRatio,256
		I2CWrite(DACRegisters::Control16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		I2CWrite(DACRegisters::Control17, 0x90); // only left DAC to left mixer enable 0db
		I2CWrite(DACRegisters::Control20, 0x90); // only right DAC to right mixer enable 0db
		I2CWrite(DACRegisters::Control21, 0x80); // set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		I2CWrite(DACRegisters::Control23, 0x00); // vroi=0

		SetDACVolume(0, 0); // 0db

		Log::WriteInfo(TAG, "Setting DAC Output: %02x", OutputMode);
		I2CWrite(DACRegisters::Power, (uint8)OutputMode);

		Log::WriteInfo(TAG, "Setting ADC Input: %02x", InputMode);
		I2CWrite(ADCRegisters::Power, 0xFF);
		I2CWrite(ADCRegisters::Control1, 0xbb); // MIC Left and Right channel PGA gain
		I2CWrite(ADCRegisters::Control2, (uint8)InputMode);

		I2CWrite(ADCRegisters::Control3, 0x02);
		I2CWrite(ADCRegisters::Control4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		I2CWrite(ADCRegisters::Control5, 0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		SetADCVolume(0, 0);					 // 0db
		I2CWrite(ADCRegisters::Power, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		return true;
	}

	static bool ConfigI2S(BitsPerSamples BitsPerSample, Formats Format) // Modules Module,
	{
		Log::WriteInfo(TAG, "Setting I2S ADC Format: %x", Format);
		I2CWrite(ADCRegisters::Control4, (I2CRead(ADCRegisters::Control4) & 0xfc) | (uint8)Format);

		Log::WriteInfo(TAG, "Setting I2S DAC Format: %x", Format);
		I2CWrite(DACRegisters::Control1, (I2CRead(DACRegisters::Control1) & 0xf9) | ((uint8)Format << 1));

		Log::WriteInfo(TAG, "Setting I2S ADC Bits: %x", BitsPerSample);
		I2CWrite(ADCRegisters::Control4, (I2CRead(ADCRegisters::Control4) & 0xe3) | ((int32)BitsPerSample << 2));

		Log::WriteInfo(TAG, "Setting I2S DAC Bits: %x", BitsPerSample);
		I2CWrite(DACRegisters::Control1, (I2CRead(DACRegisters::Control1) & 0xc7) | ((int32)BitsPerSample << 3));

		return true;
	}

	bool Start(Modules Module)
	{
		uint8 prevData = I2CRead(DACRegisters::Control21);

		if (Bitwise::IsEnabled(Module, Modules::Line))
		{
			I2CWrite(DACRegisters::Control16, 0x09); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
			I2CWrite(DACRegisters::Control17, 0x50); // left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bupass enable
			I2CWrite(DACRegisters::Control20, 0x50); // right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bupass enable
			I2CWrite(DACRegisters::Control21, 0xC0); // enable adc
		}
		else
		{
			I2CWrite(DACRegisters::Control21, 0x80); // enable dac
		}

		uint8 data = I2CRead(DACRegisters::Control21);

		if (prevData != data)
		{
			Log::WriteInfo(TAG, "Resetting State Machine");

			I2CWrite(ChipRegisters::Power, 0xF0); // start state machine
			// I2CWrite(ChipRegisters::Control1, 0x16);
			// I2CWrite(ChipRegisters::Control2, 0x50);
			// I2CWrite(ChipRegisters::Control2, 0x50);
			I2CWrite(ChipRegisters::Power, 0x00); // start state machine
		}

		if (Bitwise::IsEnabled(Module, Modules::ADC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			Log::WriteInfo(TAG, "Starting the ADC");

			I2CWrite(ADCRegisters::Power, 0x00);
		}

		if (Bitwise::IsEnabled(Module, Modules::DAC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			Log::WriteInfo(TAG, "Starting the DAC");

			I2CWrite(DACRegisters::Power, 0x3c);

			CHECK_CALL(SetMute(false));
			//- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		}

		return true;
	}

	static bool SetADCVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		I2CWrite(ADCRegisters::Control8, vol);
		I2CWrite(ADCRegisters::Control9, vol); // ADC Right Volume=0db

		return true;
	}

	static bool SetDACVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		I2CWrite(DACRegisters::Control5, vol);
		I2CWrite(DACRegisters::Control4, vol);

		return true;
	}

	static int32 GetVolume(int32 Volume, int32 Dot)
	{
		if (Volume < -96 || Volume > 0)
		{
			Log::WriteWarning(TAG, "Warning: volume < -96! or > 0!");

			if (Volume < -96)
				Volume = -96;
			else
				Volume = 0;
		}

		Dot = (Dot >= 5 ? 1 : 0);

		Volume = (-Volume << 1) + Dot;

		return Volume;
	}

	template <typename T>
	static uint8 I2CRead(T Register)
	{
		return I2CUtils::Read(ADDRESS, (uint8)Register);
	}

	template <typename T>
	static void I2CWrite(T Register, uint8 Value)
	{
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)Register, Value));
	}

private:
	InputModes m_InputMode;

	static const char *TAG;
	static const int32 ADDRESS;
};

const char *ES8388::TAG = "ES8388";
const int32 ES8388::ADDRESS = 0b0010000;

#endif