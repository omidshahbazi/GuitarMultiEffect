#pragma once

#include "framework/Common.h"

const uint32 SAMPLE_RATE = SAMPLE_RATE_48000;

const uint8 FRAME_LENGTH = 4;

typedef float SampleType;

enum class Screens
{
	Play = 0,
	Preset,
	Level,
	Rename,
	Effect,
	Rhythm,
	Save,
	Looper,
	Tuner,

	COUNT
};