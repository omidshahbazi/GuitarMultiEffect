#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "ControlManager.h"
#include "Effects/EffectManager.h"
#include "Presets/PresetManager.h"

#ifdef USE_DISPLAY
#include "Screens/ScreenManager.h"
#endif

#include "framework/DSP/Memory.h"
#include "framework/DaisySeedHAL.h"

const uint8 FRAME_LENGTH = 4;

class Application;

Application *g_Application;

class Application : public DaisySeedHAL<1, 1024>
{
public:
	Application(uint8 *SDRAMAddress = nullptr, uint32 SDRAMSize = 0)
		: DaisySeedHAL(&m_Hardware, SDRAMAddress, SDRAMSize),
		  m_ControlManager(this),
#ifdef USE_DISPLAY
		  m_ScreenManager(&m_PresetManager, &m_ControlManager),
#endif
		  m_ProcessBufferL(nullptr),
		  m_MasterVolume(0)
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
	}

	void Initialize(void)
	{
		Log::WriteInfo("Initializing");

		m_ControlManager.Initialize();

		m_ControlManager.SetBootButtonCallback({[](void *Context, float HeldTime)
												{
													daisy::System::ResetToBootloader(
#ifdef USE_SRAM
														daisy::System::BootloaderMode::DAISY_INFINITE_TIMEOUT
#else
														daisy::System::BootloaderMode::STM
#endif
													);
												}});

		m_MasterVolume = 1;
		m_ControlManager.SetVolumeCallback({this, [](void *Context, float Value)
											{
												static_cast<Application *>(Context)->m_MasterVolume = Value;
											}});

#ifdef USE_DISPLAY
		m_ControlManager.SetDisplayCallback({this, [](void *Context)
											 {
												 auto *thisPtr = static_cast<Application *>(Context);

												 thisPtr->m_ScreenManager.Render(thisPtr->m_LCDCanvas);
											 }});
#endif

		DaisySeedHAL::InitializeADC();

		PersistentBlobBase::Initialize(this);

#ifdef USE_DISPLAY
		m_LCDCanvas.Initialize(m_ControlManager.GetDisplay());
		m_LCDCanvas.SetStringSpacing(-7, 4);
#endif

		EffectManager::Initialize();

		m_PresetManager.Initialize();

#ifdef USE_DISPLAY
		m_ScreenManager.Initialize();
#endif

		m_ProcessBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

		m_Hardware.StartAudio(AudioPassthrough);
	}

	void Update(void)
	{
		DaisySeedHAL::Update();

		m_ControlManager.Update();
	}

private:
	static void AudioPassthrough(const float *const *In, float **Out, uint32 Size)
	{
		for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			g_Application->m_ProcessBufferL[i] = In[0][i];

		g_Application->m_PresetManager.Process(g_Application->m_ProcessBufferL, FRAME_LENGTH);

		for (uint32 i = 0; i < FRAME_LENGTH; ++i)
		{
			SampleType value = g_Application->m_ProcessBufferL[i] * g_Application->m_MasterVolume;

			Out[0][i] = value;
			Out[1][i] = value;
		}
	}

private:
	daisy::DaisySeed m_Hardware;
	ControlManager m_ControlManager;
	PresetManager m_PresetManager;

#ifdef USE_DISPLAY
	ScreenManager m_ScreenManager;

	LCDCanvas m_LCDCanvas;
#endif

	SampleType *m_ProcessBufferL;

	float m_MasterVolume;
};

#endif