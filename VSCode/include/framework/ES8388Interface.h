#pragma once
#ifndef ES8388_INTERFACE_H
#define ES8388_INTERFACE_H

#include "ES8388Control.h"

class ES8388Interface
{
public:
	static bool SetRegisters()
	{
		Log::WriteInfo(TAG, "Setting the Registers");

		ES8388Control::Write(ES8388Control::Registers::DACControl3, 0x04); // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp

		ES8388Control::Write(ES8388Control::Registers::ChipControl2, 0x50);

		ES8388Control::Write(ES8388Control::Registers::ChipPower, 0x00); // normal all and power up all

		ES8388Control::Write(ES8388Control::Registers::MasterMode, 0x80); // CODEC IN I2S SLAVE MODE

		ES8388Control::Write(ES8388Control::Registers::DACPower, 0xC0);		// disable DAC and disable Lout/Rout/1/2
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, 0x12); // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		ES8388Control::Write(ES8388Control::Registers::DACControl1, 0x18);	// 1a 0x18:16bit iis , 0x00:24
		ES8388Control::Write(ES8388Control::Registers::DACControl2, 0x02);	// DACFsMode,SINGLE SPEED; DACFsRatio,256
		ES8388Control::Write(ES8388Control::Registers::DACControl16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		ES8388Control::Write(ES8388Control::Registers::DACControl17, 0x90); // only left DAC to left mixer enable 0db
		ES8388Control::Write(ES8388Control::Registers::DACControl20, 0x90); // only right DAC to right mixer enable 0db
		ES8388Control::Write(ES8388Control::Registers::DACControl21, 0x80); // set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		ES8388Control::Write(ES8388Control::Registers::DACControl23, 0x00); // vroi=0

		SetDACVolume(0, 0); // 0db

		// Log::WriteInfo(TAG, "Setting DAC Output: %02x", OutputMode);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT1_1, ES8388Control::Masks::DACPower_LOUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT1_1, ES8388Control::Masks::DACPower_ROUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT2_1, ES8388Control::Masks::DACPower_LOUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT2_1, ES8388Control::Masks::DACPower_ROUT2);

		// Log::WriteInfo(TAG, "Setting ADC Input: %02x", InputMode);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, 0xFF);
		ES8388Control::Write(ES8388Control::Registers::ADCControl1, 0xbb); // MIC Left and Right channel PGA gain
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, (uint8)InputMode);

		ES8388Control::Write(ES8388Control::Registers::ADCControl3, 0x02);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, 0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		SetADCVolume(0, 0);												// 0db
		ES8388Control::Write(ES8388Control::Registers::ADCPower, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		return true;
	}

private:
	static const char *TAG;
};

const char *ES8388Interface::TAG = "ES8388";

#endif