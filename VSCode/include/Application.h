#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "ControlManager.h"
#include "framework/DSP/Memory.h"
#include "framework/DaisySeedHAL.h"
#include "framework/LCDCanvas.h"
#include "Screens/Screen.h"
#include "Screens/PlayScreen.h"

const uint32 SAMPLE_RATE = SAMPLE_RATE_48000;

const uint8 FRAME_LENGTH = 4;

const float GAIN = 1.7;

typedef float SampleType;

class Application;

Application *g_Application;

class Application : public DaisySeedHAL<256, 32>
{
public:
	Application(uint8 *SDRAMAddress = nullptr, uint32 SDRAMSize = 0)
		: DaisySeedHAL(&m_Hardware, SDRAMAddress, SDRAMSize),
		  m_ControlManager(this),
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
		m_CurrentScreen = new PlayScreen();

		Log::WriteInfo("Initializing");

		m_ControlManager.Initialize();

#ifdef DEBUG
		m_ControlManager.SetBootButtonCallback({[](void *Context, float HeldTime)
												{
													daisy::System::ResetToBootloader();
												}});
#endif

		m_MasterVolume = 1;
		m_ControlManager.SetVolumeCallback({this, [](void *Context, float Value)
											{
												static_cast<Application *>(Context)->m_MasterVolume = Value;
											}});

		m_ControlManager.SetDisplayallback({this, [](void *Context)
											{
												Application *thisPtr = static_cast<Application *>(Context);

												thisPtr->m_LCDCanvas.Clear({});

												thisPtr->m_CurrentScreen->Draw(thisPtr->m_LCDCanvas);
											}});

		DaisySeedHAL::InitializeADC();

		m_LCDCanvas.Initialize(m_ControlManager.GetDisplay());
		m_LCDCanvas.SetStringSpacing(-7, 4);

		m_ProcessBufferL = Memory::Allocate<SampleType>(FRAME_LENGTH);

		Delay(2000);

		m_Hardware.StartAudio(AudioPassthrough);
	}

	void Update(void)
	{
		DaisySeedHAL::Delay(10);

		DaisySeedHAL::Update();

		m_ControlManager.Update();
	}

private:
	static void AudioPassthrough(const float *const *In, float **Out, uint32 Size)
	{
		for (uint32 i = 0; i < FRAME_LENGTH; ++i)
			g_Application->m_ProcessBufferL[i] = In[0][i] * GAIN;

		// for (Effect<SampleType> *effect : m_Effects)
		// 	effect->Apply(m_ProcessBufferL, FRAME_LENGTH);

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

	LCDCanvas m_LCDCanvas;
	Screen *m_CurrentScreen;

	SampleType *m_ProcessBufferL;

	float m_MasterVolume;
};

#endif