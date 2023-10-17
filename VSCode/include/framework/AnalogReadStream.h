#pragma once
#ifndef ANALOG_READ_STREAM_H
#define ANALOG_READ_STREAM_H

#include "IReadStream.h"
#include "ESP32A1SModule.h"

class AnalogReadStream : public IReadStream
{
public:
	virtual bool Init(void) override
	{
		CHECK_CALL(ESP32A1SModule::Initialize(ES8388Codec::Modes::Master, ES8388Codec::ModuleVersions::V2974, ES8388::InputModes::LeftRightInput1, ES8388::OutputModes::AllLineOutputs));

		return true;
	}

	virtual bool Deinit(void) override
	{
		return true;
	}

	virtual bool Read(uint8 *Buffer, uint16 Count) override
	{
		return true;
	}
};

#endif