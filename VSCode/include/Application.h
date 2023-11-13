#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effect.h"
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
	void PassthroughTask(void);

private:
	bool m_Mute;
	EffectList m_Effects;
};

#endif