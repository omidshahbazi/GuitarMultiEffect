#include "Application.h"
#include <framework/include/DSP/Time.h>
#include <framework/include/Task.h>
#include <framework/include/DSP/Memory.h>
#include <framework/include/ESP32A1SCodec.h>
#include <framework/include/BufferUtils.h>

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

#ifdef LOOPER_EFFECT
#include "Effects/LooperEffect.h"
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
#include <framework/include/DSP/SineWaveGenerator.h>
#endif

#if defined(LOOPER_EFFECT)
const uint16 SAMPLE_RATE = SAMPLE_RATE_16000;
#elif defined(REVERB_EFFECT)
const uint16 SAMPLE_RATE = SAMPLE_RATE_16000;
#elif defined(SUSTAIN_EFFECT)
const uint16 SAMPLE_RATE = SAMPLE_RATE_16000;
#else
const uint16 SAMPLE_RATE = SAMPLE_RATE_44100;
#endif

const uint16 SAMPLE_COUNT = 64;
const uint16 FRAME_LENGTH = SAMPLE_COUNT / 2;

const float MAX_GAIN = 500;

Application::Application(void)
	: m_ControlManager(nullptr),
	  m_Mute(false)
{
	Log::Initialize(this);
	Memory::Initialize(this);

#if _DEBUG
	Log::SetMask(Log::Types::General);
#endif

	Time::Initialize();
}

void Application::Initialize(void)
{
	Log::WriteInfo("Initializing");

	ESP32A1SCodec::Configs configs;
	configs.Version = ESP32A1SCodec::Versions::V2974;
	configs.SampleRate = SAMPLE_RATE;
	configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS32;
	configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::LeftAndRight;
	configs.BufferCount = 2;
	configs.BufferLength = SAMPLE_COUNT * sizeof(int32);
	configs.InputMode = ESP32A1SCodec::InputModes::Microphone1AndMicrophone2Differential; // TODO: Not sure why, but if I use Microphone1 only, a tick noise would be present where it makes many issue in clipping effects, also I haven't switched to the differential mode on the dev-board
	configs.OutputMode = ESP32A1SCodec::OutputModes::HeadphoneLAndHeadphoneR;
	configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
	configs.EnableNoiseGate = false;
	configs.EnableAutomaticLevelControl = false;

	ESP32A1SCodec::Initialize(&configs);

	if (Bitwise::IsEnabled(configs.InputMode, ESP32A1SCodec::InputModes::Microphone1) || Bitwise::IsEnabled(configs.InputMode, ESP32A1SCodec::InputModes::Microphone2))
		ESP32A1SCodec::SetMicrophoneGain(0);

	ESP32A1SCodec::SetInputVolume(-50);
	ESP32A1SCodec::SetDigitalVolume(0);
	ESP32A1SCodec::SetOutputVolume(3);

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

#ifdef LOOPER_EFFECT
	CreateEffect<LooperEffect<SampleType>>(m_ControlManager, SAMPLE_RATE); // TODO: The memory limitation is a big issue for this one
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

	ESP32A1SCodec::PrintSystemStatistics();

	Delay(10);

#ifdef SINE_WAVE_PLAYER
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

#ifdef SINE_WAVE_PLAYER
void Application::SineWavePlayerTask(void)
{
	Log::WriteInfo("Starting SineWavePlayer Task");

	SineWaveGenerator<int32> sineWave;
	sineWave.SetDoubleBuffered(false);
	sineWave.SetSampleRate(SAMPLE_RATE);
	sineWave.SetAmplitude(0.03);
	sineWave.SetFrequency(NOTE_A4);

	uint32 bufferLen = sineWave.GetBufferLength();
	uint32 sampleCount = bufferLen * 2;

	int32 *outBuffer = Memory::Allocate<int32>(sampleCount);
	SampleType *processBufferL = Memory::Allocate<SampleType>(bufferLen);

	while (true)
	{
		for (uint16 i = 0; i < bufferLen; ++i)
			CONVERT_INT32_TO_NORMALIZED_DOUBLE(sineWave.GetBuffer(), false, 0, processBufferL, i);

		for (uint16 i = 0; i < bufferLen; ++i)
			SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, outBuffer, true, 1);

		for (Effect<SampleType> *effect : m_Effects)
			effect->Apply(processBufferL, bufferLen);

		for (uint16 i = 0; i < bufferLen; ++i)
			SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, outBuffer, true, 0);

		ESP32A1SCodec::Write(outBuffer, sampleCount, 20);
	}

	Memory::Deallocate(processBufferL);
	Memory::Deallocate(outBuffer);

	Task::Delete();
}
#else
void Application::PassthroughTask(void)
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
				processBufferL[i] *= MAX_GAIN;

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
				SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, ioBuffer, true, 1);

			for (Effect<SampleType> *effect : m_Effects)
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