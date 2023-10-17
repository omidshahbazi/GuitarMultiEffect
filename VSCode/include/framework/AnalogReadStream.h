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
		CHECK_CALL(ESP32A1SModule::Initialize(
			ESP32A1SModule::Modes::Master,
			ESP32A1SModule::Versions::V2974,
			ES8388::Modules::DAC,
			ES8388::BitLengths::BitLength16,
			ES8388::InputModes::LeftRightInput1,
			ES8388::OutputModes::AllLineOutputs,
			ES8388::Formats::Normal));

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