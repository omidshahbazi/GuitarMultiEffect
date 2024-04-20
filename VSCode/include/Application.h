#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effects/Effect.h"
#include "ControlManager.h"
#include "framework/DSP/Memory.h"
#include "framework/DaisySeedHAL.h"
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
#ifdef LOOPER_EFFECT
#include "Effects/LooperEffect.h"
#endif
#ifdef NOISE_GATE_EFFECT
#include "Effects/NoiseGateEffect.h"
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

#ifdef COMPRESSOR_EFFECT
#include "Effects/CompressorEffect.h"
#endif
#ifdef SUSTAIN_EFFECT
#include "Effects/SustainEffect.h"
#endif
#ifdef TEST_EFFECT
#include "Effects/TestEffect.h"
#endif

#ifdef SINE_WAVE_PLAYER
#include "framework/DSP/SineWaveGenerator.h"
#endif

const uint32 SAMPLE_RATE = SAMPLE_RATE_48000;

const uint8 FRAME_LENGTH = 4;

typedef float SampleType;

class Application;
Application *g_Application;
std::function<void(const float *const *In, float **Out, uint32 Size)> g_ProcessorFunction;

class Application : public DaisySeedHAL
{
private:
	typedef std::vector<Effect<SampleType> *> EffectList;

public:
	Application(uint8 *SDRAMAddress = nullptr, uint32 SDRAMSize = 0)
		: DaisySeedHAL(&m_Hardware, SDRAMAddress, SDRAMSize),
		  m_ControlManager(nullptr),
		  m_ProcessBufferL(nullptr)
	{
		Log::Initialize(this);
		Memory::Initialize(this);

		m_Hardware.Init();
		m_Hardware.SetAudioBlockSize(FRAME_LENGTH);

#ifdef _DEBUG
		Log::SetMask(Log::Types::General);
#ifdef WAIT_FOR_DEBUGGER
		m_Hardware.StartLog(true);
#endif
#endif

		daisy::SaiHandle::Config::SampleRate daisySampleRate;
		switch (SAMPLE_RATE)
		{
		case SAMPLE_RATE_8000:
			daisySampleRate = daisy::SaiHandle::Config::SampleRate::SAI_8KHZ;
			break;

		case SAMPLE_RATE_16000:
			daisySampleRate = daisy::SaiHandle::Config::SampleRate::SAI_16KHZ;
			break;

		case SAMPLE_RATE_32000:
			daisySampleRate = daisy::SaiHandle::Config::SampleRate::SAI_32KHZ;
			break;

		case SAMPLE_RATE_48000:
			daisySampleRate = daisy::SaiHandle::Config::SampleRate::SAI_48KHZ;
			break;

		case SAMPLE_RATE_96000:
			daisySampleRate = daisy::SaiHandle::Config::SampleRate::SAI_96KHZ;
			break;

		default:
			ASSERT(false, "No sutaible sample rate for %i found in the daisy", SAMPLE_RATE);
		}

		m_Hardware.SetAudioSampleRate(daisySampleRate);

		g_Application = this;
	}

	~Application(void)
	{
		if (m_ProcessBufferL != nullptr)
			Memory::Deallocate(m_ProcessBufferL);

		if (m_ControlManager != nullptr)
			Memory::Deallocate(m_ControlManager);
	}

	void Initialize(void)
	{
		Log::WriteInfo("Initializing");

		m_ControlManager = Memory::Allocate<ControlManager>();
		new (m_ControlManager) ControlManager(this);

#ifdef AUTO_WAH_EFFECT
		CreateEffect<AutoWahEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef CHORUS_EFFECT
		CreateEffect<ChorusEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef DISTORTION_EFFECT
		CreateEffect<DistortionEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef FLANGER_EFFECT
		CreateEffect<FlangerEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef LOOPER_EFFECT
		CreateEffect<LooperEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef NOISE_GATE_EFFECT
		CreateEffect<NoiseGateEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef OVERDRIVE_EFFECT
		CreateEffect<OverdriveEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef PHASER_EFFECT
		CreateEffect<PhaserEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef REVERB_EFFECT
		CreateEffect<ReverbEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef TREMOLO_EFFECT
		CreateEffect<TremoloEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif
#ifdef WAH_EFFECT
		CreateEffect<WahEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif

#ifdef COMPRESSOR_EFFECT
		CreateEffect<CompressorEffect<SampleType>>(m_ControlManager, SAMPLE_RATE); // TODO: Algorithm seems incorrect
#endif
#ifdef SUSTAIN_EFFECT
		CreateEffect<SustainEffect<SampleType>>(m_ControlManager, SAMPLE_RATE); // TODO: Does it work?
#endif

#ifdef TEST_EFFECT
		CreateEffect<TestEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif

		// Tube Screamer
		// Octave

		DaisySeedHAL::InitializeADC();

#ifdef SINE_WAVE_PLAYER
		InitializeSineWavePlayer();
#else
		InitializeAudioPassthrough();
#endif

		Delay(2000);

		auto audioCallback = [](const float *const *In, float **Out, uint32 Size)
		{
			ASSERT(g_ProcessorFunction != nullptr, "g_ProcessorFunction cannot be null");

			g_ProcessorFunction(In, Out, Size);
		};
		m_Hardware.StartAudio(audioCallback);
	}

	void Update(void)
	{
		DaisySeedHAL::Update();

		m_ControlManager->Update();
	}

private:
#ifdef SINE_WAVE_PLAYER
	void InitializeSineWavePlayer(void)
	{
		Log::WriteInfo("Starting Sin eWave Player");

		SineWaveGenerator<int32> sineWave;
		sineWave.SetDoubleBuffered(false);
		sineWave.SetSampleRate(SAMPLE_RATE);
		sineWave.SetAmplitude(1);
		sineWave.SetFrequency(NOTE_A4);

		const uint32 bufferLen = sineWave.GetBufferLength();

		m_ProcessBufferL = Memory::Allocate<SampleType>(bufferLen);

		uint32 bufferIndex = 0;

		g_ProcessorFunction = [&](const float *const *In, float **Out, uint32 Size)
		{
			SampleType *processBufferChunk = m_ProcessBufferL + bufferIndex;

			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
				float value = sineWave.GetBuffer()[bufferIndex + i] / (float)SineWaveGenerator<int32>::MAX_VALUE;

				processBufferChunk[i] = value;
				Out[1][i] = value;
			}

			for (Effect<SampleType> *effect : m_Effects)
				effect->Apply(processBufferChunk, FRAME_LENGTH);

			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
				Out[0][i] = processBufferChunk[i];

			bufferIndex = (bufferIndex + FRAME_LENGTH) % bufferLen;
		};
	}
#else
	void InitializeAudioPassthrough(void)
	{
		m_ProcessBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

		Log::WriteInfo("Starting Audio Passthrough");

		g_ProcessorFunction = [&](const float *const *In, float **Out, uint32 Size)
		{
			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
				m_ProcessBufferL[i] = In[0][i];

			for (Effect<SampleType> *effect : m_Effects)
				effect->Apply(m_ProcessBufferL, FRAME_LENGTH);

			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
				Out[0][i] = m_ProcessBufferL[i];
				Out[1][i] = In[0][i];
			}
		};
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
	daisy::DaisySeed m_Hardware;
	ControlManager *m_ControlManager;
	EffectList m_Effects;
	SampleType *m_ProcessBufferL;
};

#endif