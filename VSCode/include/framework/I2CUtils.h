#pragma once
#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include "Common.h"
#include "ESPDebug.h"
#include <driver/i2c.h>

class I2CUtils
{
public:
	static bool WriteBulk(uint8 BusAddress, uint8 Register, uint8 *Data, uint8 Length)
	{
		const uint8 MASTER_NUM = 0;
		const uint8 ACK_CHECK_EN = 0x1;

		// printf("Writing [%02x]=", Register);
		// for (int i = 0; i < Length; i++)
		// 	printf("%02x:", Data[i]);
		// printf("\n");

		i2c_cmd_handle_t cmd = i2c_cmd_link_create();

		ESP_CHECK_CALL(i2c_master_start(cmd));

		ESP_CHECK_CALL(i2c_master_write_byte(cmd, BusAddress << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_write(cmd, &Register, 1, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_write(cmd, Data, Length, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_stop(cmd));
		ESP_CHECK_CALL(i2c_master_cmd_begin(MASTER_NUM, cmd, 1000 / portTICK_RATE_MS));

		i2c_cmd_link_delete(cmd);

		return true;
	}

	static bool Write(uint8 BusAddress, uint8 Register, uint8 Value)
	{
		return WriteBulk(BusAddress, Register, &Value, 1);
	}
};

#endif