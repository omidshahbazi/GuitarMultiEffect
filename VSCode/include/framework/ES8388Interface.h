#pragma once
#ifndef ES8388_INTERFACE_H
#define ES8388_INTERFACE_H

#include "ES8388Control.h"
#include "Math.h"

class ES8388Interface
{
public:
	static bool Initialize()
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

		// SetDACVolume(0, 0); // 0db

		Log::WriteInfo(TAG, "Setting DAC Output");
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT1_1, ES8388Control::Masks::DACPower_LOUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT1_1, ES8388Control::Masks::DACPower_ROUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT2_1, ES8388Control::Masks::DACPower_LOUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT2_1, ES8388Control::Masks::DACPower_ROUT2);

		Log::WriteInfo(TAG, "Setting ADC Input");
		ES8388Control::Write(ES8388Control::Registers::ADCPower, 0xFF);
		ES8388Control::Write(ES8388Control::Registers::ADCControl1, 0xbb); // MIC Left and Right channel PGA gain
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_DSR_0, ES8388Control::Masks::ADCControl2_DSR);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_DSSEL_0, ES8388Control::Masks::ADCControl2_DSSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_RINSEL_00, ES8388Control::Masks::ADCControl2_RINSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_LINSEL_00, ES8388Control::Masks::ADCControl2_RINSEL);

		ES8388Control::Write(ES8388Control::Registers::ADCControl3, 0x02);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, 0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		// SetADCVolume(0, 0);												// 0db
		ES8388Control::Write(ES8388Control::Registers::ADCPower, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		Log::WriteInfo(TAG, "Setting I2S ADC Format");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCFORMAT_00, ES8388Control::Masks::ADCControl4_ADCFORMAT);

		Log::WriteInfo(TAG, "Setting I2S DAC Format");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACFORMAT_00, ES8388Control::Masks::DACControl1_DACFORMAT);

		Log::WriteInfo(TAG, "Setting I2S ADC Bits");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCWL_100, ES8388Control::Masks::ADCControl4_ADCWL);

		Log::WriteInfo(TAG, "Setting I2S DAC Bits");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACWL_100, ES8388Control::Masks::DACControl1_DACWL);

		SetOutputVolume(14.5F);

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

	//[-45dB, 4.5dB]
	static bool SetOutputVolume(float dB)
	{
		dB = Math::Clamp(dB, -45, 4.5F);

		Log::WriteInfo(TAG, "Setting the volume: %fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)((uint8)((dB + 45) / 1.5F) & (uint8)ES8388Control::Masks::DACControl24_LOUT1VOL);

		ES8388Control::Write(ES8388Control::Registers::DACControl24, (ES8388Control::Values)value, ES8388Control::Masks::DACControl24_LOUT1VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl25, (ES8388Control::Values)value, ES8388Control::Masks::DACControl25_ROUT1VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl26, (ES8388Control::Values)value, ES8388Control::Masks::DACControl26_LOUT2VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl27, (ES8388Control::Values)value, ES8388Control::Masks::DACControl27_ROUT2VOL);

		return true;
	}

	static float GetOutputVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl24, ES8388Control::Masks::DACControl24_LOUT1VOL);

		return ((uint8)value * 1.5F) - 45;
	}

private:
	static const char *TAG;
};

const char *ES8388Interface::TAG = "ES8388Interface";

#endif