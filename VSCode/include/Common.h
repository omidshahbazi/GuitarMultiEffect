#pragma once

#include "framework/Common.h"

const uint32 SAMPLE_RATE = SAMPLE_RATE_48000;

typedef float SampleType;

enum class Screens
{
	Play = 0,
	Preset,
	Rename,
	Effect,
	Rhythm,
	Save,

	COUNT
};