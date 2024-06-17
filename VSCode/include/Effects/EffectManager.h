#pragma once
#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"
#include "FXEffect.h"
#include "DSEffect.h"
#include "AmpEffect.h"
#include "EqEffect.h"
#include "ModEffect.h"
#include "DelEffect.h"
#include "RevEffect.h"
#include "../framework/DSP/Memory.h"

class EffectManager
{
public:
	enum class Types
	{
#ifdef ADD_FX_EFFECT
		FX = 0,
#endif
#ifdef ADD_DS_EFFECT
		Ds,
#endif
#ifdef ADD_AMP_EFFECT
		Amp,
#endif
#ifdef ADD_EQ_EFFECT
		Eq,
#endif
#ifdef ADD_MOD_EFFECT
		Mod,
#endif
#ifdef ADD_DEL_EFFECT
		Del,
#endif
#ifdef ADD_REV_EFFECT
		Rev,
#endif

		COUNT
	};

public:
	static void Initialize(void)
	{
#ifdef ADD_FX_EFFECT
		Add<FXEffect>(Types::FX);
#endif
#ifdef ADD_DS_EFFECT
		Add<DsEffect>(Types::Ds);
#endif
#ifdef ADD_AMP_EFFECT
		Add<AmpEffect>(Types::Amp);
#endif
#ifdef ADD_EQ_EFFECT
		Add<EqEffect>(Types::Eq);
#endif
#ifdef ADD_MOD_EFFECT
		Add<ModEffect>(Types::Mod);
#endif
#ifdef ADD_DEL_EFFECT
		Add<DelEffect>(Types::Del);
#endif
#ifdef ADD_REV_EFFECT
		Add<RevEffect>(Types::Rev);
#endif
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