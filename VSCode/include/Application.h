#define SINE_WAVE_PLAYER

#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effects/Effect.h"
#include "ControlManager.h"
#include "framework/include/Common.h"
#include <vector>

class Application
{
private:
	typedef std::vector<Effect *> EffectList;

public:
	Application(void);

	void Initialize(void);

private:
#ifdef SINE_WAVE_PLAYER
	void SineWavePlayerTask(void);
#else
	void PassthroughTask(void);
#endif

private:
	bool m_Mute;
	ControlManager m_ControlManager;
	EffectList m_Effects;
};

#endif