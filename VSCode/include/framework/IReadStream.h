#pragma once
#ifndef I_READ_STREAM_H
#define I_READ_STREAM_H

#include "Common.h"

class IReadStream
{
public:
	virtual bool Init(void) = 0;
	virtual bool Deinit(void) = 0;

	virtual bool Read(uint8 *Buffer, uint16 Count) = 0;
};

#endif