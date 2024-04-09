#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effects/Effect.h"
#include "ControlManager.h"
#include <framework/include/Common.h>
#include <vector>

typedef float SampleType;

class Application
{
private:
	typedef std::vector<Effect<SampleType> *> EffectList;

public:
	Application(void);

	void Initialize(void);

private:
#ifdef SINE_WAVE_PLAYER
	void SineWavePlayerTask(void);
#else
	void PassthroughTask(void);
#endif

	template <typename EffectType, typename... ArgsT>
	EffectType *CreateEffect(ArgsT... Args)
	{
		EffectType *effect = Memory::Allocate<EffectType>(1, true);

		new (effect) EffectType(Args...);

		m_Effects.push_back(effect);

		return effect;
	}

private:
	bool m_Mute;
	ControlManager m_ControlManager;
	EffectList m_Effects;
};

#endif