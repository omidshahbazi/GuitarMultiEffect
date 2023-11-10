#pragma once
#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H

#include "Effect.h"
#include "framework/include/Memory.h"

class DelayEffect : public Effect
{
public:
	DelayEffect(uint8 FrameLength)
		: m_Buffer(nullptr),
		  m_BufferLength(FrameLength * 2),
		  m_BufferIndex(FrameLength)
	{
		m_Buffer = Memory::Allocate<double>(m_BufferLength);
	}

	~DelayEffect(void)
	{
		Memory::Deallocate(m_Buffer);
	}

protected:
	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double input = Buffer[i];

			Buffer[i] = m_Buffer[m_BufferIndex];

			m_Buffer[m_BufferIndex] = input;

			m_BufferIndex = (m_BufferIndex + 1) % m_BufferLength;
		}
	}

private:
	double *m_Buffer;
	uint8 m_BufferLength;
	uint8 m_BufferIndex;
};

#endif