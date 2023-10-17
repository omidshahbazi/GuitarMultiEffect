#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#define KHz 1000
#define MHz 1000 * KHz

class Bitwise
{
public:
	template <typename T>
	static bool IsEnabled(T Mask, T Bits)
	{
		return (((int32)Mask & (int32)Bits) == (int32)Bits);
	}
};

template <typename T, typename U>
T operator|(T Left, U Right)
{
	return (T)((uint32)Left | (uint32)Right);
}

template <typename T, typename U>
T operator|=(T Left, U Right)
{
	return (T)((uint32)Left | (uint32)Right);
}

#endif