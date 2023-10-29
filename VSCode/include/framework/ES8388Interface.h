#pragma once
#ifndef ES8388_INTERFACE_H
#define ES8388_INTERFACE_H

#include "ES8388Control.h"
#include "Math.h"

class ES8388Interface
{
public:
	static bool Initialize(void)
	{
		Log::WriteInfo(TAG, "Setting the Registers");

		SetOutputMute(true);

		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnVrefbuf_0, ES8388Control::Masks::ChipControl2_PdnVrefbuf);
		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_VrefLo_0, ES8388Control::Masks::ChipControl2_VrefLo);
		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnIbiasgen_0, ES8388Control::Masks::ChipControl2_PdnIbiasgen);
		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnAna_0, ES8388Control::Masks::ChipControl2_PdnAna);
		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVrefBuf_1, ES8388Control::Masks::ChipControl2_LPVrefBuf);
		ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVcmMod_0, ES8388Control::Masks::ChipControl2_LPVcmMod);

		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dacVref_PDN_0, ES8388Control::Masks::ChipPower_dacVref_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adcVref_PDN_0, ES8388Control::Masks::ChipPower_adcVref_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_DACDLL_PDN_0, ES8388Control::Masks::ChipPower_DACDLL_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_ADCDLL_PDN_0, ES8388Control::Masks::ChipPower_ADCDLL_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_stm_rst_0, ES8388Control::Masks::ChipPower_dac_stm_rst);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_stm_rst_0, ES8388Control::Masks::ChipPower_adc_stm_rst);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_DigPDN_0, ES8388Control::Masks::ChipPower_dac_DigPDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_DigPDN_0, ES8388Control::Masks::ChipPower_adc_DigPDN);

		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLKDIV_00000, ES8388Control::Masks::MasterMode_BCLKDIV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLK_INV_0, ES8388Control::Masks::MasterMode_BCLK_INV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_MCLKDIV2_0, ES8388Control::Masks::MasterMode_MCLKDIV2);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_MSC_0, ES8388Control::Masks::MasterMode_MSC);

		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT2_0, ES8388Control::Masks::DACPower_ROUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT2_0, ES8388Control::Masks::DACPower_LOUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT1_0, ES8388Control::Masks::DACPower_ROUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT1_0, ES8388Control::Masks::DACPower_LOUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACR_1, ES8388Control::Masks::DACPower_PdnDACR);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACL_1, ES8388Control::Masks::DACPower_PdnDACL);

		//  Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_VMIDSEL_10, ES8388Control::Masks::ChipControl1_VMIDSEL);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_EnRef_0, ES8388Control::Masks::ChipControl1_EnRef);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SeqEn_0, ES8388Control::Masks::ChipControl1_SeqEn);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SameFs_1, ES8388Control::Masks::ChipControl1_SameFs);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_DACMCLK_0, ES8388Control::Masks::ChipControl1_DACMCLK);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_LRCM_0, ES8388Control::Masks::ChipControl1_LRCM);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SCPReset_0, ES8388Control::Masks::ChipControl1_SCPReset);

		//  1a 0x18:16bit iis , 0x00:24
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACFORMAT_00, ES8388Control::Masks::DACControl1_DACFORMAT);
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACWL_011, ES8388Control::Masks::DACControl1_DACWL);
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACLRP_0, ES8388Control::Masks::DACControl1_DACLRP);
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACLRSWAP_0, ES8388Control::Masks::DACControl1_DACLRSWAP);

		//  DACFsMode,SINGLE SPEED; DACFsRatio,256
		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsRatio_00010, ES8388Control::Masks::DACCOntrol2_DACFsRatio);
		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsMode_0, ES8388Control::Masks::DACCOntrol2_DACFsMode);

		//  0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_RMIXSEL_000, ES8388Control::Masks::DACControl16_RMIXSEL);
		ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_LMIXSEL_000, ES8388Control::Masks::DACControl16_LMIXSEL);

		//  only left DAC to left mixer enable 0db
		ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LO_0, ES8388Control::Masks::DACControl17_LI2LO);
		ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LD2LO_1, ES8388Control::Masks::DACControl17_LD2LO);

		//  only right DAC to right mixer enable 0db
		// ES8388Control::Write(ES8388Control::Registers::DACControl20, 0x90);
		ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2RO_0, ES8388Control::Masks::DACControl20_RI2RO);
		ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RD2RO_1, ES8388Control::Masks::DACControl20_RD2RO);

		SetInputToMixerGain(6);

		//  set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_dac_dll_pwd_0, ES8388Control::Masks::DACControl21_dac_dll_pwd);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_adc_dll_pwd_0, ES8388Control::Masks::DACControl21_adc_dll_pwd);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_mclk_dis_0, ES8388Control::Masks::DACControl21_mclk_dis);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_offset_dis_0, ES8388Control::Masks::DACControl21_offset_dis);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_lrck_sel_0, ES8388Control::Masks::DACControl21_lrck_sel);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_slrck_1, ES8388Control::Masks::DACControl21_slrck);

		//  vroi=0
		ES8388Control::Write(ES8388Control::Registers::DACControl23, ES8388Control::Values::DACControl23_VROI_0, ES8388Control::Masks::DACControl23_VROI);

		SetDACVolume(0);

		Log::WriteInfo(TAG, "Setting DAC Output");
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT2_1, ES8388Control::Masks::DACPower_ROUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT2_1, ES8388Control::Masks::DACPower_LOUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT1_1, ES8388Control::Masks::DACPower_ROUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT1_1, ES8388Control::Masks::DACPower_LOUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACR_0, ES8388Control::Masks::DACPower_PdnDACR);
		ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACL_0, ES8388Control::Masks::DACPower_PdnDACL);

		Log::WriteInfo(TAG, "Setting ADC Input");

		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_int1LP_1, ES8388Control::Masks::ADCPower_int1LP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_flashLP_1, ES8388Control::Masks::ADCPower_flashLP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCBiasgen_1, ES8388Control::Masks::ADCPower_PdnADCBiasgen);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnMICB_1, ES8388Control::Masks::ADCPower_PdnMICB);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCR_1, ES8388Control::Masks::ADCPower_PdnADCR);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCL_1, ES8388Control::Masks::ADCPower_PdnADCL);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINR_1, ES8388Control::Masks::ADCPower_PdnAINR);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINL_1, ES8388Control::Masks::ADCPower_PdnAINL);

		//  MIC Left and Right channel PGA gain
		SetMicrophoneGain(24);

		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_DSR_0, ES8388Control::Masks::ADCControl2_DSR);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_DSSEL_0, ES8388Control::Masks::ADCControl2_DSSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_LINSEL_01, ES8388Control::Masks::ADCControl2_LINSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_RINSEL_10, ES8388Control::Masks::ADCControl2_RINSEL);

		//  0x02 doesn't exist, is required?
		ES8388Control::Write(ES8388Control::Registers::ADCControl3, ES8388Control::Values::ADCControl3_TRI_0, ES8388Control::Masks::ADCControl3_TRI);
		ES8388Control::Write(ES8388Control::Registers::ADCControl3, ES8388Control::Values::ADCControl3_MONOMIX_00, ES8388Control::Masks::ADCControl3_MONOMIX);
		ES8388Control::Write(ES8388Control::Registers::ADCControl3, ES8388Control::Values::ADCControl3_DS_0, ES8388Control::Masks::ADCControl3_DS);

		//  Left/Right data, Left/Right justified mode, Bits length, I2S format
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCFORMAT_01, ES8388Control::Masks::ADCControl4_ADCFORMAT);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCWL_011, ES8388Control::Masks::ADCControl4_ADCWL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCLRP_0, ES8388Control::Masks::ADCControl4_ADCLRP);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_DATSEL_00, ES8388Control::Masks::ADCControl4_DATSEL);

		//  ADCFsMode,singel SPEED,RATIO=256
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsRatio_00010, ES8388Control::Masks::ADCControl5_ADCFsRatio);
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsMode_0, ES8388Control::Masks::ADCControl5_ADCFsMode);

		// ALC for Microphone
		SetADCVolume(0);

		//  Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_int1LP_1, ES8388Control::Masks::ADCPower_int1LP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_flashLP_0, ES8388Control::Masks::ADCPower_flashLP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCBiasgen_0, ES8388Control::Masks::ADCPower_PdnADCBiasgen);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnMICB_1, ES8388Control::Masks::ADCPower_PdnMICB);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCR_0, ES8388Control::Masks::ADCPower_PdnADCR);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCL_0, ES8388Control::Masks::ADCPower_PdnADCL);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINR_0, ES8388Control::Masks::ADCPower_PdnAINR);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINL_0, ES8388Control::Masks::ADCPower_PdnAINL);

		Log::WriteInfo(TAG, "Setting I2S ADC Format");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCFORMAT_00, ES8388Control::Masks::ADCControl4_ADCFORMAT);

		Log::WriteInfo(TAG, "Setting I2S DAC Format");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACFORMAT_00, ES8388Control::Masks::DACControl1_DACFORMAT);

		Log::WriteInfo(TAG, "Setting I2S ADC Bits");
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCWL_001, ES8388Control::Masks::ADCControl4_ADCWL);

		Log::WriteInfo(TAG, "Setting I2S DAC Bits");
		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACWL_011, ES8388Control::Masks::DACControl1_DACWL);

		SetOutputVolume(4.5F);

		uint8 prevData = ES8388Control::Read(ES8388Control::Registers::DACControl21);

		if (0) // Bitwise::IsEnabled(Module, Modules::Line)
		{
			// 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_RMIXSEL_001, ES8388Control::Masks::DACControl16_RMIXSEL);
			ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_LMIXSEL_001, ES8388Control::Masks::DACControl16_LMIXSEL);

			// left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bypass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LOVOL_000, ES8388Control::Masks::DACControl17_LI2LOVOL);
			ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LO_1, ES8388Control::Masks::DACControl17_LI2LO);
			ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LD2LO_0, ES8388Control::Masks::DACControl17_LD2LO);

			// right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bypass enable
			ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2ROVOL_000, ES8388Control::Masks::DACControl20_RI2ROVOL);
			ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2RO_1, ES8388Control::Masks::DACControl20_RI2RO);
			ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RD2RO_0, ES8388Control::Masks::DACControl20_RD2RO);

			SetInputToMixerGain(0);

			// enable dac
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_dac_dll_pwd_0, ES8388Control::Masks::DACControl21_dac_dll_pwd);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_adc_dll_pwd_0, ES8388Control::Masks::DACControl21_adc_dll_pwd);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_mclk_dis_0, ES8388Control::Masks::DACControl21_mclk_dis);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_offset_dis_0, ES8388Control::Masks::DACControl21_offset_dis);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_lrck_sel_1, ES8388Control::Masks::DACControl21_lrck_sel);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_slrck_1, ES8388Control::Masks::DACControl21_slrck);
		}
		else
		{
			//  enable dac
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_dac_dll_pwd_0, ES8388Control::Masks::DACControl21_dac_dll_pwd);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_adc_dll_pwd_0, ES8388Control::Masks::DACControl21_adc_dll_pwd);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_mclk_dis_0, ES8388Control::Masks::DACControl21_mclk_dis);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_offset_dis_0, ES8388Control::Masks::DACControl21_offset_dis);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_lrck_sel_0, ES8388Control::Masks::DACControl21_lrck_sel);
			ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_slrck_1, ES8388Control::Masks::DACControl21_slrck);
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
			Log::WriteInfo(TAG, "Starting ADC");

			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_int1LP_0, ES8388Control::Masks::ADCPower_int1LP);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_flashLP_0, ES8388Control::Masks::ADCPower_flashLP);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCBiasgen_0, ES8388Control::Masks::ADCPower_PdnADCBiasgen);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnMICB_0, ES8388Control::Masks::ADCPower_PdnMICB);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCR_0, ES8388Control::Masks::ADCPower_PdnADCR);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnADCL_0, ES8388Control::Masks::ADCPower_PdnADCL);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINR_0, ES8388Control::Masks::ADCPower_PdnAINR);
			ES8388Control::Write(ES8388Control::Registers::ADCPower, ES8388Control::Values::ADCPower_PdnAINL_0, ES8388Control::Masks::ADCPower_PdnAINL);
		}

		// if (Bitwise::IsEnabled(Module, Modules::DAC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			Log::WriteInfo(TAG, "Starting DAC");

			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT2_1, ES8388Control::Masks::DACPower_ROUT2);
			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT2_1, ES8388Control::Masks::DACPower_LOUT2);
			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_ROUT1_1, ES8388Control::Masks::DACPower_ROUT1);
			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_LOUT1_1, ES8388Control::Masks::DACPower_LOUT1);
			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACR_0, ES8388Control::Masks::DACPower_PdnDACR);
			ES8388Control::Write(ES8388Control::Registers::DACPower, ES8388Control::Values::DACPower_PdnDACL_0, ES8388Control::Masks::DACPower_PdnDACL);

			CHECK_CALL(SetOutputMute(false));
		}

		return true;
	}

	static bool TurnOnChip(void)
	{
		return true;
	}

	static bool TurnOffChip(void)
	{
		return true;
	}

	// static bool TurnOnChip(void)
	// {
	// return true;
	// }

	// static bool TurnOffChip(void)
	// {
	// return true;
	// }

	static bool SetADCBitsPerSample()
	{
		return true;
	}

	static bool SetDACBitsPerSample()
	{
		return true;
	}

	//[0dB, 24dB]
	static bool SetMicrophoneGain(float dB)
	{
		dB = Math::Clamp(dB, 0, 24);

		Log::WriteInfo(TAG, "Setting the Microphone gain: %fdB", dB);

		uint8 value = (uint8)(dB / 3);

		ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)(value & (uint8)ES8388Control::Masks::ADCControl1_MicAmpR), ES8388Control::Masks::ADCControl1_MicAmpR);
		ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)((value << 4) & (uint8)ES8388Control::Masks::ADCControl1_MicAmpL), ES8388Control::Masks::ADCControl1_MicAmpL);

		return true;
	}

	static float GetMicrophoneGain(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl1, ES8388Control::Masks::ADCControl1_MicAmpR);

		return (uint8)value * 3.0F;
	}

	//[-15dB, 6dB]
	static bool SetInputToMixerGain(float dB)
	{
		dB = Math::Clamp(dB, -15, 6);

		Log::WriteInfo(TAG, "Setting Input to Mixer gain: %fdB", dB);

		uint8 value = 7 - (uint8)((dB + 15) / 3.0F);

		ES8388Control::Write(ES8388Control::Registers::DACControl17, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl17_LI2LOVOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl20, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl20_RI2ROVOL);

		return true;
	}

	static float GetInputToMixerGain(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl17, ES8388Control::Masks::DACControl17_LI2LOVOL);

		return ((7 - (uint8)value) * 3.0F) - 15;
	}

	// 	static void SetMicrophoneGain(float dB)
	// {
	// }

	// static float GetMicrophoneGain(void)
	// {
	// }

	//[-96dB, 0dB]
	static bool SetADCVolume(float dB)
	{
		dB = Math::Clamp(dB, -96, 0);

		Log::WriteInfo(TAG, "Setting ADC volume: %fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * 2)) & (uint8)ES8388Control::Masks::ADCControl8_LADCVOL);

		ES8388Control::Write(ES8388Control::Registers::ADCControl8, value, ES8388Control::Masks::ADCControl8_LADCVOL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl9, value, ES8388Control::Masks::ADCControl9_RADCVOL);

		return true;
	}

	static float GetADCVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl8, ES8388Control::Masks::ADCControl8_LADCVOL);

		return (uint8)value / 2.0F;
	}

	//[-96dB, 0dB]
	static bool SetDACVolume(float dB)
	{
		dB = Math::Clamp(dB, -96, 0);

		Log::WriteInfo(TAG, "Setting DAC volume: %fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * 2)) & (uint8)ES8388Control::Masks::DACControl4_LDACVOL);

		ES8388Control::Write(ES8388Control::Registers::DACControl4, value, ES8388Control::Masks::DACControl4_LDACVOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl5, value, ES8388Control::Masks::DACControl5_RDACVOL);

		return true;
	}

	static float GetDACVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl4, ES8388Control::Masks::DACControl4_LDACVOL);

		return (uint8)value / 2.0F;
	}

	//[-45dB, 4.5dB]
	static bool SetOutputVolume(float dB)
	{
		dB = Math::Clamp(dB, -45, 4.5F);

		Log::WriteInfo(TAG, "Setting Output volume: %fdB", dB);

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

	static bool SetOutputMute(bool Mute)
	{
		Log::WriteInfo(TAG, "Setting Mute: %i", Mute);

		ES8388Control::Write(
			ES8388Control::Registers::DACControl3,
			(Mute ? ES8388Control::Values::DACControl3_DACMute_1 : ES8388Control::Values::DACControl3_DACMute_0),
			ES8388Control::Masks::DACControl3_DACMute);

		return true;
	}

	static bool GetOutputMute(void)
	{
		return (ES8388Control::Read(ES8388Control::Registers::DACControl3, ES8388Control::Masks::DACControl3_DACMute) == ES8388Control::Values::DACControl3_DACMute_1);
	}

private:
	static const char *TAG;
};

const char *ES8388Interface::TAG = "ES8388Interface";

#endif