#pragma once
#ifndef ESP32_A1S_MODULE_H
#define ESP32_A1S_MODULE_H

#include "ES8388.h"

class ESP32A1SModule
{
public:
	enum class Modes
	{
		Master = 0,
		Slave
	};

	enum class Versions
	{
		V2957 = 0,
		V2974
	};

public:
	static bool Initialize(Modes Mode, Versions Version, ES8388::Modules Module, ES8388::BitLengths BitLength, ES8388::InputModes InputMode, ES8388::OutputModes OutputMode, ES8388::Formats Format)
	{
		Print(ESP_LOG_INFO, "Tag", "Initialize1");
		CHECK_CALL(I2CInitialize(Mode, Version));

		Print(ESP_LOG_INFO, "Tag", "Initialize2");
		CHECK_CALL(SetI2SPin(Version));

		Print(ESP_LOG_INFO, "Tag", "Initialize3");
		CHECK_CALL(ES8388::Initialize(Module, BitLength, InputMode, OutputMode, Format));

		return true;
	}

private:
	static bool I2CInitialize(Modes Mode, Versions Version)
	{
		i2c_config_t config = {};
		config.mode = (Mode == Modes::Master ? I2C_MODE_MASTER : I2C_MODE_SLAVE);
		config.sda_pullup_en = GPIO_PULLUP_ENABLE;
		config.scl_pullup_en = GPIO_PULLUP_ENABLE;
		config.master.clk_speed = 100000;

		if (Version == Versions::V2957)
		{
			config.sda_io_num = GPIO_NUM_33;
			config.scl_io_num = GPIO_NUM_32;
		}
		else if (Version == Versions::V2974)
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

	static bool SetI2SPin(Versions Version)
	{
		i2s_pin_config_t config = {};
		config.ws_io_num = GPIO_NUM_25;
		config.data_out_num = GPIO_NUM_26;
		config.data_in_num = GPIO_NUM_35;

		gpio_num_t masterClockPin = GPIO_NUM_0;

		if (Version == Versions::V2957)
		{
			config.bck_io_num = GPIO_NUM_27;
			masterClockPin = GPIO_NUM_3;
		}
		else if (Version == Versions::V2974)
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
		ASSERT(port != I2S_NUM_MAX, "Setting MasterClockPin", "Does not support I2S_NUM_MAX");
		ASSERT(gpio == GPIO_NUM_0 || gpio == GPIO_NUM_1 || gpio == GPIO_NUM_3, "Setting MasterClockPin", "GPIO_NUM_0, GPIO_NUM_1 and GPIO_NUM_3 are only supported for master");

		LOG_INFO(FRAMEWORK_TAG, "Setting master clock for I2S%d on GPIO%d", port, gpio);

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