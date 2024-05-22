#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "OverdriveEffect.h"
#include "../framework/DSP/Memory.h"

class EffectManager
{
public:
	enum class Types
	{
		Overdrive = 0,

		COUNT
	};

public:
	static void Initialize(void)
	{
		Add<OverdriveEffect>(Types::Overdrive);
	}

	template <typename T>
	static T *GetEffect(Types Type)
	{
		return reinterpret_cast<T *>(*Get(Type));
	}

private:
	template <typename T>
	static void Add(Types Type)
	{
		T *effect = Memory::Allocate<T>(1, true);
		new (effect) T();

		*Get(Type) = effect;
	}

	static Effect **Get(Types Type)
	{
		static Effect *effects[(uint8)Types::COUNT] = {};

		return &effects[(uint8)Types::Overdrive];
	}
};

#endif
#pragma GCC pop_options