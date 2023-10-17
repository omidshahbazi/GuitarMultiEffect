#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "Common.h"

template <typename T>
T *Allocate(uint16 Count = 1)
{
	T *mem = Allocate<T>(sizeof(T) * Count);

	memset(mem, 0, sizeof(T) * Count);

	return mem;
}

#endif