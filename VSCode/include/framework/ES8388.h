#pragma once
#ifndef ES8388_H
#define ES8388_H

#include "Debug.h"
#include "ESPDebug.h"
#include "I2CUtils.h"
#include <driver/i2s.h>
#include <driver/i2c.h>

class ES8388
{
public:
	enum class InputModes
	{
		None = -1,
		LeftRightInput1 = 0x00,
		Microphone1 = 0x05,
		Microphone2 = 0x06,
		LeftRightInput2 = 0x50,
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
	static bool Initialize(InputModes InputMode, OutputModes OutputMode)
	{
		CHECK_CALL(SetRegisters(InputMode, OutputMode));

		// Modes Available
		//
		//	es_mode_t  = ES_MODULE_ADC;
		//	es_mode_t  = ES_MODULE_LINE;
		//	es_mode_t  = ES_MODULE_DAC;
		//	es_mode_t  = ES_MODULE_ADC_DAC;

		es_bits_length_t bits_length = BIT_LENGTH_16BITS;
		es_module_t module = ES_MODULE_DAC;
		es_format_t fmt = I2S_NORMAL;

		es8388_config_i2s(bits_length, ES_MODULE_ADC_DAC, fmt);
		es8388_set_voice_volume(70);
		es8388_start(module);
	}

private:
	static bool SetRegisters(InputModes InputMode, OutputModes OutputMode)
	{

		enum class MasterModes
		{
			None = -1,
			Slave = 0x00,
			Master = 0x01
		};

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control3, 0x04)); // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ChipRegisters::Control2, 0x50));
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ChipRegisters::Power, 0x00)); // normal all and power up all

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ChipRegisters::Mode, (uint8)MasterModes::Slave)); // CODEC IN I2S SLAVE MODE

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Power, 0xC0));		// disable DAC and disable Lout/Rout/1/2
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ChipRegisters::Control1, 0x12)); // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control1, 0x18));	// 1a 0x18:16bit iis , 0x00:24
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control2, 0x02));	// DACFsMode,SINGLE SPEED; DACFsRatio,256
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control16, 0x00)); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control17, 0x90)); // only left DAC to left mixer enable 0db
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control20, 0x90)); // only right DAC to right mixer enable 0db
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control21, 0x80)); // set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Control23, 0x00)); // vroi=0

		SetDACVolume(0, 0); // 0db

		LOG_INFO(TAG, "Setting DAC Output: %02x", OutputMode);
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)DACRegisters::Power, (uint8)OutputMode));

		LOG_INFO(TAG, "Setting ADC Input: %02x", InputMode);
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Power, 0xFF));
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Control1, 0xbb)); // MIC Left and Right channel PGA gain
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Control2, (uint8)InputMode));

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Control3, 0x02));
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Control4, 0x0d)); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)ADCRegisters::Control5, 0x02)); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		SetADCVolume(0, 0);														 // 0db
		CHECK_CALL(I2CUtils::Writeg(ADDRESS, (uint8)ADCRegisters::Power, 0x09)); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		return true;
	}

	static bool SetADCVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		CHECK_CALL(I2CUtils::Writeg(ADDRESS, (uint8)ADCRegisters::Control8, vol));
		CHECK_CALL(I2CUtils::Writeg(ADDRESS, (uint8)ADCRegisters::Control9, vol)); // ADC Right Volume=0db

		return true;
	}

	static int SetDACVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		CHECK_CALL(I2CUtils::Writeg(ADDRESS, (uint8)DACRegisters::Control5, vol));
		CHECK_CALL(I2CUtils::Writeg(ADDRESS, (uint8)DACRegisters::Control4, vol));

		return true;
	}

private:
	static int32 GetVolume(int32 Volume, int32 Dot)
	{
		if (Volume < -96 || Volume > 0)
		{
			LOG_WARNING(TAG, "Warning: volume < -96! or > 0!");

			if (Volume < -96)
				Volume = -96;
			else
				Volume = 0;
		}

		Dot = (Dot >= 5 ? 1 : 0);

		Volume = (-Volume << 1) + Dot;

		return Volume;
	}

private:
	static const int32 ADDRESS;
};

const int32 ES8388::ADDRESS = 0b0010000;

#endif