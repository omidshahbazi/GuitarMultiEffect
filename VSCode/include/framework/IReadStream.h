#ifndef I_READ_STREAM_H
#define I_READ_STREAM_H

#include <inttypes.h>

class IReadStream
{
public:
	virtual bool Init(void) = 0;
	virtual bool Deinit(void) = 0;

	virtual bool Read(uint8_t *Buffer, uint16_t Count) = 0;
};

#endif