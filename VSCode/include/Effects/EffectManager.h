#pragma once
#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "FXEffect.h"
#include "DSEffect.h"
#include "EqEffect.h"
#include "RevEffect.h"
#include "ModEffect.h"
#include "../framework/DSP/Memory.h"

class EffectManager
{
public:
	enum class Types
	{
		FX = 0,
		Ds,
		// Amp,
		Eq,
		Mod,
		// Del,
		Rev,

		COUNT
	};

public:
	static void Initialize(void)
	{
		Add<FXEffect>(Types::FX);
		Add<DsEffect>(Types::Ds);
		Add<EqEffect>(Types::Eq);
		Add<RevEffect>(Types::Rev);
		Add<ModEffect>(Types::Mod);
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

		return &effects[(uint8)Type];
	}
};

#endif