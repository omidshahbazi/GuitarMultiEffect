#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "IEffect.h"
#include "framework/include/Common.h"

class Application
{
public:
	Application(void);

	void Initialize(void);

	void Update(void);

private:
	void PassthroughTask(void);

	static void PassthroughTask(void *arg)
	{
		reinterpret_cast<Application *>(arg)->PassthroughTask();
	}

private:
	bool m_Mute;
	float m_OutCorrectionGain;
	IEffect *m_Effects[8];
	? ? ? ?
};

#endif