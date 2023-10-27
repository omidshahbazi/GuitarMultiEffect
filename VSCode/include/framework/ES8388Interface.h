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

		ES8388Control::Write(ES8388Control::Registers::MasterMode, 0x00); // CODEC IN I2S SLAVE MODE

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
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_LINSEL_01, ES8388Control::Masks::ADCControl2_LINSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_RINSEL_10, ES8388Control::Masks::ADCControl2_RINSEL);

		ES8388Control::Write(ES8388Control::Registers::ADCControl3, 0x02);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, 0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		SetADCVolume(0, 0); // 0db

		ES8388Control::Write(ES8388Control::Registers::ADCPower, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		Log::WriteInfo(TAG, "Setting I2S ADC Format");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCFORMAT_00, ES8388Control::Masks::ADCControl4_ADCFORMAT);

		Log::WriteInfo(TAG, "Setting I2S DAC Format");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACFORMAT_00, ES8388Control::Masks::DACControl1_DACFORMAT);

		Log::WriteInfo(TAG, "Setting I2S ADC Bits");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCWL_001, ES8388Control::Masks::ADCControl4_ADCWL);

		Log::WriteInfo(TAG, "Setting I2S DAC Bits");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACWL_001, ES8388Control::Masks::DACControl1_DACWL);

		SetOutputVolume(4.5F);

		uint8 prevData = ES8388Control::Read(ES8388Control::Registers::DACControl21);

		if (0) // Bitwise::IsEnabled(Module, Modules::Line)
		{
			// 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_RMIXSEL_001, ES8388Control::Masks::DACControl16_RMIXSEL);
			ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_LMIXSEL_001, ES8388Control::Masks::DACControl16_LMIXSEL);

			// left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bupass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LO_1, ES8388Control::Masks::DACControl17_LI2LO);
			ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LOVOL_010, ES8388Control::Masks::DACControl17_LI2LOVOL);

			// right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bupass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2RO_1, ES8388Control::Masks::DACControl20_RI2RO);
			ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2ROVOL_010, ES8388Control::Masks::DACControl20_RI2ROVOL);

			// enable adc
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_lrck_sel_1, ES8388Control::Masks::DACControl21_lrck_sel);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_slrck_1, ES8388Control::Masks::DACControl21_slrck);
		}
		else
		{
			ES8388Control::Write(ES8388Control::Registers::DACControl21, 0x80); // enable dac
		}

		uint8 data = ES8388Control::Read(ES8388Control::Registers::DACControl21);

		if (prevData != data)
		{
			Log::WriteInfo(TAG, "Resetting State Machine");

			// start state machine
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_stm_rst_1, ES8388Control::Masks::ChipPower_dac_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_stm_rst_1, ES8388Control::Masks::ChipPower_adc_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_DigPDN_1, ES8388Control::Masks::ChipPower_dac_DigPDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_DigPDN_1, ES8388Control::Masks::ChipPower_adc_DigPDN);

			// I2CWrite(ChipRegisters::Control1, 0x16);
			// I2CWrite(ChipRegisters::Control2, 0x50);
			// I2CWrite(ChipRegisters::Control2, 0x50);

			// start state machine
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dacVref_PDN_0, ES8388Control::Masks::ChipPower_dacVref_PDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adcVref_PDN_0, ES8388Control::Masks::ChipPower_adcVref_PDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_DACDLL_PDN_0, ES8388Control::Masks::ChipPower_DACDLL_PDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_ADCDLL_PDN_0, ES8388Control::Masks::ChipPower_ADCDLL_PDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_stm_rst_0, ES8388Control::Masks::ChipPower_dac_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_stm_rst_0, ES8388Control::Masks::ChipPower_adc_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_DigPDN_0, ES8388Control::Masks::ChipPower_dac_DigPDN);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_DigPDN_0, ES8388Control::Masks::ChipPower_adc_DigPDN);
		}

		// if (Bitwise::IsEnabled(Module, Modules::ADC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			Log::WriteInfo(TAG, "Starting the ADC");

			ES8388Control::Write(ES8388Control::Registers::ADCPower, 0x00);
		}

		// if (Bitwise::IsEnabled(Module, Modules::DAC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			Log::WriteInfo(TAG, "Starting the DAC");

			ES8388Control::Write(ES8388Control::Registers::DACPower, 0x3c);

			CHECK_CALL(SetMute(false));
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

	static bool SetMute(bool Mute)
	{
		Log::WriteInfo(TAG, "Setting the Mute: %i", Mute);

		ES8388Control::Write(
			ES8388Control::Registers::DACControl3,
			(Mute ? ES8388Control::Values::DACControl3_DACMute_1 : ES8388Control::Values::DACControl3_DACMute_0),
			ES8388Control::Masks::DACControl3_DACMute);

		return true;
	}

	static bool GetMute(void)
	{
		return (ES8388Control::Read(ES8388Control::Registers::DACControl3, ES8388Control::Masks::DACControl3_DACMute) == ES8388Control::Values::DACControl3_DACMute_1);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool SetADCVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		ES8388Control::Write(ES8388Control::Registers::ADCControl8, vol);
		ES8388Control::Write(ES8388Control::Registers::ADCControl9, vol); // ADC Right Volume=0db

		return true;
	}

	static bool SetDACVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		ES8388Control::Write(ES8388Control::Registers::DACControl5, vol);
		ES8388Control::Write(ES8388Control::Registers::DACControl4, vol);

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

private:
	static const char *TAG;
};

const char *ES8388Interface::TAG = "ES8388Interface";

#endif