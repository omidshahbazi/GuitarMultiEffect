#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effects/Effect.h"
#include "ControlManager.h"
#include <framework/include/ESP32HAL.h>
#include <framework/include/Task.h>
#include <framework/include/DSP/Memory.h>
#include <framework/include/ESP32A1SCodec.h>
#include <framework/include/BufferUtils.h>
#include <vector>

#ifdef AUTO_WAH_EFFECT
#include "Effects/AutoWahEffect.h"
#endif
#ifdef CHORUS_EFFECT
#include "Effects/ChorusEffect.h"
#endif
#ifdef DISTORTION_EFFECT
#include "Effects/DistortionEffect.h"
#endif
#ifdef FLANGER_EFFECT
#include "Effects/FlangerEffect.h"
#endif
#ifdef OVERDRIVE_EFFECT
#include "Effects/OverdriveEffect.h"
#endif
#ifdef PHASER_EFFECT
#include "Effects/PhaserEffect.h"
#endif
#ifdef REVERB_EFFECT
#include "Effects/ReverbEffect.h"
#endif
#ifdef TREMOLO_EFFECT
#include "Effects/TremoloEffect.h"
#endif
#ifdef WAH_EFFECT
#include "Effects/WahEffect.h"
#endif

#ifdef TEST_EFFECT
#include "Effects/TestEffect.h"
#endif

#ifdef PLAY_SINE_WAVE
#include <framework/include/DSP/Filters/OscillatorFilter.h>
#include <framework/include/DSP/Notes.h>
#endif

#if defined(LOOPER_EFFECT)
const uint16 SAMPLE_RATE = SAMPLE_RATE_16000;
#elif defined(REVERB_EFFECT)
const uint16 SAMPLE_RATE = SAMPLE_RATE_16000;
#else
const uint16 SAMPLE_RATE = SAMPLE_RATE_44100;
#endif

const uint16 FRAME_LENGTH = 4;
const uint16 SAMPLE_COUNT = FRAME_LENGTH * 2;

const float GAIN = 1.0;

typedef float SampleType;

class Application : public ESP32HAL<0, 0>
{
private:
	typedef std::vector<Effect<SampleType, SAMPLE_RATE> *> EffectList;

public:
	Application(void)
		: m_ControlManager(nullptr),
		  m_Mute(false)
	{
		Log::Initialize(this);
		Memory::Initialize(this);

#ifdef DEBUG
		Log::SetMask(Log::Types::General);
#endif
	}

	void Initialize(void)
	{
		Log::WriteInfo("Initializing");

		ESP32A1SCodec::Configs configs;
		configs.Version = ESP32A1SCodec::Versions::V2974;
		configs.SampleRate = SAMPLE_RATE;
		configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS32;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::LeftAndRight;
		configs.BufferCount = 2;
		configs.BufferLength = SAMPLE_COUNT * sizeof(int32);
		configs.InputMode = ESP32A1SCodec::InputModes::Microphone1;
		configs.OutputMode = ESP32A1SCodec::OutputModes::HeadphoneLAndHeadphoneR;
		configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
		configs.EnableNoiseGate = false;
		configs.EnableAutomaticLevelControl = false;

		ESP32A1SCodec::Initialize(&configs);

		if (Bitwise::IsEnabled(configs.InputMode, ESP32A1SCodec::InputModes::Microphone1) || Bitwise::IsEnabled(configs.InputMode, ESP32A1SCodec::InputModes::Microphone2))
			ESP32A1SCodec::SetMicrophoneGain(12);

		ESP32A1SCodec::SetInputVolume(-2.5);
		ESP32A1SCodec::SetDigitalVolume(0);
		ESP32A1SCodec::SetOutputVolume(-28.5);

		m_ControlManager = Memory::Allocate<ControlManager>();
		new (m_ControlManager) ControlManager(this);

#ifdef AUTO_WAH_EFFECT
		CreateEffect<AutoWahEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef CHORUS_EFFECT
		CreateEffect<ChorusEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef DISTORTION_EFFECT
		CreateEffect<DistortionEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef FLANGER_EFFECT
		CreateEffect<FlangerEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef OVERDRIVE_EFFECT
		CreateEffect<OverdriveEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef PHASER_EFFECT
		CreateEffect<PhaserEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef REVERB_EFFECT
		CreateEffect<ReverbEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef TREMOLO_EFFECT
		CreateEffect<TremoloEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif
#ifdef WAH_EFFECT
		CreateEffect<WahEffect<SampleType, SAMPLE_RATE>>(m_ControlManager);
#endif

#ifdef TEST_EFFECT
		CreateEffect<TestEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif

		ESP32A1SCodec::PrintSystemStatistics();

		Delay(10);

#ifdef PLAY_SINE_WAVE
		Task::Create(
			[this]()
			{
				SineWavePlayerTask();
			},
			4096, "SineWavePlayerTask", 1, 10);
#else
		Task::Create(
			[this]()
			{
				PassthroughTask();
			},
			4096, "PassthroughTask", 1, 10);
#endif
	}

private:
#ifdef PLAY_SINE_WAVE
	void SineWavePlayerTask(void)
	{
		Log::WriteInfo("Starting SineWavePlayer Task");

		OscillatorFilter<SampleType, SAMPLE_RATE> oscillator;
		oscillator.SetFrequency(NOTE_A4);

		int32 *outBuffer = Memory::Allocate<int32>(SAMPLE_COUNT);
		SampleType *processBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

		while (true)
		{
			oscillator.ProcessBuffer(processBufferL, FRAME_LENGTH);

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
				SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, outBuffer, true, 1);

			for (Effect<SampleType, SAMPLE_RATE> *effect : m_Effects)
				effect->Apply(processBufferL, FRAME_LENGTH);

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				ASSERT(Math::Absolute(processBufferL[i]) <= 1, "Processed value is out of range: %f", processBufferL[i]);

				SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, outBuffer, true, 0);
			}

			ESP32A1SCodec::Write(outBuffer, SAMPLE_COUNT, 20);
		}

		Memory::Deallocate(processBufferL);
		Memory::Deallocate(outBuffer);

		Task::Delete();
	}
#else
	void PassthroughTask(void)
	{
		Log::WriteInfo("Starting Passthrough Task");

		Delay(2000);

		int32 *ioBuffer = Memory::Allocate<int32>(SAMPLE_COUNT);
		SampleType *processBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

		while (true)
		{
			if (m_Mute)
			{
				Memory::Set(processBufferL, 0, FRAME_LENGTH);
			}
			else
			{
				ESP32A1SCodec::Read(ioBuffer, SAMPLE_COUNT, 20);

				for (uint16 i = 0; i < FRAME_LENGTH; ++i)
					CONVERT_INT32_TO_NORMALIZED_DOUBLE(ioBuffer, true, 0, processBufferL, i);

				for (uint16 i = 0; i < FRAME_LENGTH; ++i)
				{
					processBufferL[i] *= GAIN;

					ASSERT(Math::Absolute(processBufferL[i]) <= 1, "Gained input value is out of range: %f", processBufferL[i]);
				}

				for (uint16 i = 0; i < FRAME_LENGTH; ++i)
				{
					ASSERT(Math::Absolute(processBufferL[i]) <= 1, "Processed value is out of range: %f", processBufferL[i]);

					SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, ioBuffer, true, 1);
				}

				for (Effect<SampleType, SAMPLE_RATE> *effect : m_Effects)
					effect->Apply(processBufferL, FRAME_LENGTH);
			}

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
				SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, ioBuffer, true, 0);

			ESP32A1SCodec::Write(ioBuffer, SAMPLE_COUNT, 20);
		}

		Memory::Deallocate(processBufferL);
		Memory::Deallocate(ioBuffer);

		Task::Delete();
	}
#endif

	template <typename EffectType, typename... ArgsT>
	EffectType *CreateEffect(ArgsT... Args)
	{
		EffectType *effect = Memory::Allocate<EffectType>();

		new (effect) EffectType(Args...);

		m_Effects.push_back(effect);

		return effect;
	}

private:
	bool m_Mute;
	ControlManager *m_ControlManager;
	EffectList m_Effects;
};

#endif