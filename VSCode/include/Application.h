#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "framework/IReadStream.h"

class Application
{
public:
	Application(void);

	void Initialize(void);

	void Update(void);

private:
	void SetupI2S(void);
	void I2SRoutine(void);

	static void I2SRoutine(void *arg)
	{
		reinterpret_cast<Application *>(arg)->I2SRoutine();
	}

private:
	IReadStream *m_ReadStream;

	int32 *m_InBufferInt;
	float *m_InBuffer;

	int32 *m_OutBufferInt;
	float *m_OutBuffer;
};

#endif