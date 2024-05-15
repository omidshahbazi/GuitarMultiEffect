#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Effects/Effect.h"
#include "ControlManager.h"
#include "framework/DSP/Memory.h"
#include "framework/DaisySeedHAL.h"
#include <vector>

#ifdef DEBUG
#include "framework/DSP/SampleAmountMeter.h"
#endif

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
#include "framework/DSP/Filters/OscillatorFilter.h"
#include "framework/DSP/Notes.h"
#endif

const uint32 SAMPLE_RATE = SAMPLE_RATE_48000;

const uint8 FRAME_LENGTH = 4;

const float GAIN = 1.7;

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

#ifdef DEBUG
		Log::SetMask(Log::Types::General);
#ifdef WAIT_FOR_DEBUGGER
		m_Hardware.StartLog(true);
#else
		m_Hardware.StartLog(false);
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

#ifdef DEBUG
		Button *bootModeButton = m_ControlManager->CreateButton("Boot Mode Button", GPIOPins::Pin30);
		bootModeButton->SetOnTurnedOffListener(
			[&](float HeldTime)
			{
				m_Hardware.system.ResetToBootloader();
			});
#endif

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

#ifdef TEST_EFFECT
		CreateEffect<TestEffect<SampleType>>(m_ControlManager, SAMPLE_RATE);
#endif

		DaisySeedHAL::InitializeADC();

		m_ProcessBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

#ifdef PLAY_SINE_WAVE
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

#ifdef DEBUG
		ASSERT(m_InputSampleAmountMeter.GetAbsoluteMax() <= 1, "Gained Input value is out of range: %f", m_InputSampleAmountMeter.GetAbsoluteMax());
		m_InputSampleAmountMeter.Reset();

		ASSERT(m_OutputSampleAmountMeter.GetAbsoluteMax() <= 1, "Processed value is out of range: %f", m_OutputSampleAmountMeter.GetAbsoluteMax());
		m_OutputSampleAmountMeter.Reset();
#endif
	}

private:
#ifdef PLAY_SINE_WAVE
	void InitializeSineWavePlayer(void)
	{
		Log::WriteInfo("Starting Sine Wave Player");

		OscillatorFilter<SampleType> *oscillator = Memory::Allocate<OscillatorFilter<SampleType>>();
		new (oscillator) OscillatorFilter<SampleType>(SAMPLE_RATE);
		oscillator->SetFrequency(NOTE_A4);

		g_ProcessorFunction = [&, oscillator](const float *const *In, float **Out, uint32 Size)
		{
			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
				float value = oscillator->Process(1);

				m_ProcessBufferL[i] = value;
				Out[1][i] = value;
			}

			for (Effect<SampleType> *effect : m_Effects)
				effect->Apply(m_ProcessBufferL, FRAME_LENGTH);

			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
#ifdef DEBUG
				m_OutputSampleAmountMeter.Record(m_ProcessBufferL[i]);
#endif
				Out[0][i] = m_ProcessBufferL[i];
			}
		};
	}
#else
	void InitializeAudioPassthrough(void)
	{
		Log::WriteInfo("Starting Audio Passthrough");

		g_ProcessorFunction = [&](const float *const *In, float **Out, uint32 Size)
		{
			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
				m_ProcessBufferL[i] = In[0][i] * GAIN;

#ifdef DEBUG
				m_InputSampleAmountMeter.Record(m_ProcessBufferL[i]);
#endif
			}

			for (Effect<SampleType> *effect : m_Effects)
				effect->Apply(m_ProcessBufferL, FRAME_LENGTH);

			for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			{
#ifdef DEBUG
				m_OutputSampleAmountMeter.Record(m_ProcessBufferL[i]);
#endif

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

#ifdef DEBUG
	SampleAmountMeter m_InputSampleAmountMeter;
	SampleAmountMeter m_OutputSampleAmountMeter;
#endif
};

#endif