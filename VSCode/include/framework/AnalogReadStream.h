#pragma once
#ifndef ANALOG_READ_STREAM_H
#define ANALOG_READ_STREAM_H

#include "IReadStream.h"
#include "ES8388Codec.h"

class AnalogReadStream : public IReadStream
{
public:
	virtual bool Init(void) override
	{
		CHECK_CALL(ES8388Codec::I2CInitialize(ES8388Codec::Modes::Master, ES8388Codec::ModuleVersions::V2974));

		return true;
	}

	virtual bool Deinit(void) override
	{
		return true;
	}

	virtual bool Read(uint8_t *Buffer, uint16_t Count) override
	{
		return true;
	}
};

#endif