#pragma once
#ifndef ESP32_A1S_MODULE_H
#define ESP32_A1S_MODULE_H

#include "ES8388.h"

// /* ES8388 register */
// #define ES8388_CONTROL1 0x00
// #define ES8388_CONTROL2 0x01

// #define ES8388_CHIPPOWER 0x02

// #define ES8388_ADCPOWER 0x03
// #define ES8388_DACPOWER 0x04

// #define ES8388_CHIPLOPOW1 0x05
// #define ES8388_CHIPLOPOW2 0x06

// #define ES8388_ANAVOLMANAG 0x07

// #define ES8388_MASTERMODE 0x08
// /* ADC */
// #define ES8388_ADCCONTROL1 0x09
// #define ES8388_ADCCONTROL2 0x0a
// #define ES8388_ADCCONTROL3 0x0b
// #define ES8388_ADCCONTROL4 0x0c
// #define ES8388_ADCCONTROL5 0x0d
// #define ES8388_ADCCONTROL6 0x0e
// #define ES8388_ADCCONTROL7 0x0f
// #define ES8388_ADCCONTROL8 0x10
// #define ES8388_ADCCONTROL9 0x11
// #define ES8388_ADCCONTROL10 0x12
// #define ES8388_ADCCONTROL11 0x13
// #define ES8388_ADCCONTROL12 0x14
// #define ES8388_ADCCONTROL13 0x15
// #define ES8388_ADCCONTROL14 0x16
// /* DAC */
// #define ES8388_DACCONTROL1 0x17
// #define ES8388_DACCONTROL2 0x18
// #define ES8388_DACCONTROL3 0x19
// #define ES8388_DACCONTROL4 0x1a
// #define ES8388_DACCONTROL5 0x1b
// #define ES8388_DACCONTROL6 0x1c
// #define ES8388_DACCONTROL7 0x1d
// #define ES8388_DACCONTROL8 0x1e
// #define ES8388_DACCONTROL9 0x1f
// #define ES8388_DACCONTROL10 0x20
// #define ES8388_DACCONTROL11 0x21
// #define ES8388_DACCONTROL12 0x22
// #define ES8388_DACCONTROL13 0x23
// #define ES8388_DACCONTROL14 0x24
// #define ES8388_DACCONTROL15 0x25
// #define ES8388_DACCONTROL16 0x26
// #define ES8388_DACCONTROL17 0x27
// #define ES8388_DACCONTROL18 0x28
// #define ES8388_DACCONTROL19 0x29
// #define ES8388_DACCONTROL20 0x2a
// #define ES8388_DACCONTROL21 0x2b
// #define ES8388_DACCONTROL22 0x2c
// #define ES8388_DACCONTROL23 0x2d
// #define ES8388_DACCONTROL24 0x2e
// #define ES8388_DACCONTROL25 0x2f
// #define ES8388_DACCONTROL26 0x30
// #define ES8388_DACCONTROL27 0x31
// #define ES8388_DACCONTROL28 0x32
// #define ES8388_DACCONTROL29 0x33
// #define ES8388_DACCONTROL30 0x34

// typedef enum
// {
// 	I2S_NORMAL = 0, /*!< set normal I2S format */
// 	I2S_LEFT,		/*!< set all left format */
// 	I2S_RIGHT,		/*!< set all right format */
// 	I2S_DSP,		/*!< set dsp/pcm format */
// } es_format_t;

// typedef enum
// {
// 	BIT_LENGTH_MIN = -1,
// 	BIT_LENGTH_16BITS = 0x03,
// 	BIT_LENGTH_18BITS = 0x02,
// 	BIT_LENGTH_20BITS = 0x01,
// 	BIT_LENGTH_24BITS = 0x00,
// 	BIT_LENGTH_32BITS = 0x04,
// 	BIT_LENGTH_MAX,
// } es_bits_length_t;

// typedef enum
// {
// 	MCLK_DIV_MIN = -1,
// 	MCLK_DIV_1 = 1,
// 	MCLK_DIV_2 = 2,
// 	MCLK_DIV_3 = 3,
// 	MCLK_DIV_4 = 4,
// 	MCLK_DIV_6 = 5,
// 	MCLK_DIV_8 = 6,
// 	MCLK_DIV_9 = 7,
// 	MCLK_DIV_11 = 8,
// 	MCLK_DIV_12 = 9,
// 	MCLK_DIV_16 = 10,
// 	MCLK_DIV_18 = 11,
// 	MCLK_DIV_22 = 12,
// 	MCLK_DIV_24 = 13,
// 	MCLK_DIV_33 = 14,
// 	MCLK_DIV_36 = 15,
// 	MCLK_DIV_44 = 16,
// 	MCLK_DIV_48 = 17,
// 	MCLK_DIV_66 = 18,
// 	MCLK_DIV_72 = 19,
// 	MCLK_DIV_5 = 20,
// 	MCLK_DIV_10 = 21,
// 	MCLK_DIV_15 = 22,
// 	MCLK_DIV_17 = 23,
// 	MCLK_DIV_20 = 24,
// 	MCLK_DIV_25 = 25,
// 	MCLK_DIV_30 = 26,
// 	MCLK_DIV_32 = 27,
// 	MCLK_DIV_34 = 28,
// 	MCLK_DIV_7 = 29,
// 	MCLK_DIV_13 = 30,
// 	MCLK_DIV_14 = 31,
// 	MCLK_DIV_MAX,
// } es_sclk_div_t;

// typedef enum
// {
// 	LCLK_DIV_MIN = -1,
// 	LCLK_DIV_128 = 0,
// 	LCLK_DIV_192 = 1,
// 	LCLK_DIV_256 = 2,
// 	LCLK_DIV_384 = 3,
// 	LCLK_DIV_512 = 4,
// 	LCLK_DIV_576 = 5,
// 	LCLK_DIV_768 = 6,
// 	LCLK_DIV_1024 = 7,
// 	LCLK_DIV_1152 = 8,
// 	LCLK_DIV_1408 = 9,
// 	LCLK_DIV_1536 = 10,
// 	LCLK_DIV_2112 = 11,
// 	LCLK_DIV_2304 = 12,

// 	LCLK_DIV_125 = 16,
// 	LCLK_DIV_136 = 17,
// 	LCLK_DIV_250 = 18,
// 	LCLK_DIV_272 = 19,
// 	LCLK_DIV_375 = 20,
// 	LCLK_DIV_500 = 21,
// 	LCLK_DIV_544 = 22,
// 	LCLK_DIV_750 = 23,
// 	LCLK_DIV_1000 = 24,
// 	LCLK_DIV_1088 = 25,
// 	LCLK_DIV_1496 = 26,
// 	LCLK_DIV_1500 = 27,
// 	LCLK_DIV_MAX,
// } es_lclk_div_t;

// typedef enum
// {
// 	D2SE_PGA_GAIN_MIN = -1,
// 	D2SE_PGA_GAIN_DIS = 0,
// 	D2SE_PGA_GAIN_EN = 1,
// 	D2SE_PGA_GAIN_MAX = 2,
// } es_d2se_pga_t;

// typedef enum
// {
// 	MIC_GAIN_MIN = -1,
// 	MIC_GAIN_0DB = 0,
// 	MIC_GAIN_3DB = 3,
// 	MIC_GAIN_6DB = 6,
// 	MIC_GAIN_9DB = 9,
// 	MIC_GAIN_12DB = 12,
// 	MIC_GAIN_15DB = 15,
// 	MIC_GAIN_18DB = 18,
// 	MIC_GAIN_21DB = 21,
// 	MIC_GAIN_24DB = 24,
// 	MIC_GAIN_MAX,
// } es_mic_gain_t;

// typedef enum
// {
// 	ES_MODULE_MIN = -1,
// 	ES_MODULE_ADC = 0x01,
// 	ES_MODULE_DAC = 0x02,
// 	ES_MODULE_ADC_DAC = 0x03,
// 	ES_MODULE_LINE = 0x04,
// 	ES_MODULE_MAX
// } es_module_t;

// typedef enum
// {
// 	ES_MODE_MIN = -1,
// 	ES_MODE_SLAVE = 0x00,
// 	ES_MODE_MASTER = 0x01,
// 	ES_MODE_MAX,
// } es_mode_t;

// typedef enum
// {
// 	ES_I2S_MIN = -1,
// 	ES_I2S_NORMAL = 0,
// 	ES_I2S_LEFT = 1,
// 	ES_I2S_RIGHT = 2,
// 	ES_I2S_DSP = 3,
// 	ES_I2S_MAX
// } es_i2s_fmt_t;

// /**
//  * @brief Configure ES8388 clock
//  */
// typedef struct
// {
// 	es_sclk_div_t sclk_div; /*!< bits clock divide */
// 	es_lclk_div_t lclk_div; /*!< WS clock divide */
// } es_i2s_clock_t;

class ESP32A1SModule
{
public:
	enum class Modes
	{
		Master = 0,
		Slave
	};

	enum class ModuleVersions
	{
		V2957 = 0,
		V2974
	};

public:
	static bool Initialize(Modes Mode, ModuleVersions ModuleVersion, ES8388::InputModes InputMode, ES8388::OutputModes OutputMode)
	{
		CHECK_CALL(I2CInitialize(Mode, ModuleVersion));

		CHECK_CALL(SetI2SPin(ModuleVersion));

		CHECK_CALL(ES8388::Initialize(InputMode, OutputMode));
	}

private:
	static bool I2CInitialize(Modes Mode, ModuleVersions ModuleVersion)
	{
		i2c_config_t config = {};
		config.mode = (Mode == Modes::Master ? I2C_MODE_MASTER : I2C_MODE_SLAVE);
		config.sda_pullup_en = GPIO_PULLUP_ENABLE;
		config.scl_pullup_en = GPIO_PULLUP_ENABLE;
		config.master.clk_speed = 100000;

		if (ModuleVersion == ModuleVersions::V2957)
		{
			config.sda_io_num = GPIO_NUM_33;
			config.scl_io_num = GPIO_NUM_32;
		}
		else if (ModuleVersion == ModuleVersions::V2974)
		{
			config.sda_io_num = GPIO_NUM_18;
			config.scl_io_num = GPIO_NUM_23;
		}
		else
			return false;

		ESP_CHECK_CALL(i2c_param_config(I2S_NUM_0, &config));

		ESP_CHECK_CALL(i2c_driver_install(I2S_NUM_0, config.mode, 0, 0, 0));

		return true;
	}

	static bool SetI2SPin(ModuleVersions ModuleVersion)
	{
		i2s_pin_config_t config = {};
		config.ws_io_num = GPIO_NUM_25;
		config.data_out_num = GPIO_NUM_26;
		config.data_in_num = GPIO_NUM_35;

		gpio_num_t masterClockPin = GPIO_NUM_0;

		if (ModuleVersion == ModuleVersions::V2957)
		{
			config.bck_io_num = GPIO_NUM_27;
			masterClockPin = GPIO_NUM_3;
		}
		else if (ModuleVersion == ModuleVersions::V2974)
		{
			config.bck_io_num = GPIO_NUM_5;
			masterClockPin = GPIO_NUM_0;
		}
		else
			return false;

		ESP_CHECK_CALL(i2s_set_pin(I2S_NUM_0, &config));

		SetMasterClockPin(I2S_NUM_0, masterClockPin);

		return true;
	}

	static void SetMasterClockPin(i2s_port_t port, gpio_num_t gpio)
	{
		ASSERT(port != I2S_NUM_MAX, "Does not support I2S_NUM_MAX");
		ASSERT(gpio == GPIO_NUM_0 || gpio == GPIO_NUM_1 || gpio == GPIO_NUM_3, "GPIO_NUM_0, GPIO_NUM_1 and GPIO_NUM_3 are only supported for master");

		LOG_INFO(TAG, "Setting master clock for I2S%d on GPIO%d", port, gpio);

		if (port == I2S_NUM_0)
		{
			if (gpio == GPIO_NUM_0)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
				WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
			}
			else if (gpio == GPIO_NUM_1)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
				WRITE_PERI_REG(PIN_CTRL, 0xF0F0);
			}
			else
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
				WRITE_PERI_REG(PIN_CTRL, 0xFF00);
			}
		}
		else if (port == I2S_NUM_1)
		{
			if (gpio == GPIO_NUM_0)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
				WRITE_PERI_REG(PIN_CTRL, 0xFFFF);
			}
			else if (gpio == GPIO_NUM_1)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
				WRITE_PERI_REG(PIN_CTRL, 0xF0FF);
			}
			else
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
				WRITE_PERI_REG(PIN_CTRL, 0xFF0F);
			}
		}
	}

private:
	static const char *TAG;
};

#endif