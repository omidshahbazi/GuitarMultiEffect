#include "Application.h"
#include <framework/include/Time.h>
#include <framework/include/Task.h>
#include <framework/include/Memory.h>
#include <framework/include/ESP32A1SCodec.h>
#include <framework/include/BufferUtils.h>

#ifdef AUTO_WAH_EFFECT
#include "Effects/AutoWahEffect.h"
#endif
#ifdef CHORUS_EFFECT
#include "Effects/ChorusEffect.h"
#endif
#ifdef COMPRESSOR_EFFECT
#include "Effects/CompressorEffect.h"
#endif
#ifdef DISTORTION_EFFECT
#include "Effects/DistortionEffect.h"
#endif
#ifdef NOISE_GATE_EFFECT
#include "Effects/NoiseGateEffect.h"
#endif
#ifdef OVERDRIVE_EFFECT
#include "Effects/OverdriveEffect.h"
#endif
#ifdef REVERB_EFFECT
#include "Effects/ReverbEffect.h"
#endif
#ifdef SUSTAIN_EFFECT
#include "Effects/SustainEffect.h"
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
#ifdef SINE_WAVE_PLAYER
#include <framework/include/SineWaveGenerator.h>
#endif

const uint16 SAMPLE_RATE = SAMPLE_RATE_44100;
const uint16 SAMPLE_COUNT = 64;
const uint16 FRAME_LENGTH = SAMPLE_COUNT / 2;

template <typename T, typename... ArgsT>
T *CreateEffect(Application::EffectList &Effects, ArgsT... Args)
{
	T *effect = Memory::Allocate<T>(1, true);

	new (effect) T(Args...);

	Effects.push_back(effect);

	return effect;
}

Application::Application(void)
	: m_Mute(false),
	  m_ControlManager(GPIOPins::Pin13)
{
#if __PLATFORMIO_BUILD_DEBUG__
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
	configs.BufferCount = 3;
	configs.BufferLength = 300;
	configs.InputMode = ESP32A1SCodec::InputModes::Microphone1;
	configs.OutputMode = ESP32A1SCodec::OutputModes::HeadphoneL;
	configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
	configs.EnableNoiseGate = true;
	configs.EnableAutomaticLevelControl = false;

	ESP32A1SCodec::Initialize(&configs);

	// TODO: To be able to tune values, bind a button to Log enable/disable, so I can tune the values, then turn-on the logs and see the value

#ifdef AUTO_WAH_EFFECT
	CreateEffect<AutoWahEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif
#ifdef CHORUS_EFFECT
	CreateEffect<ChorusEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif
#ifdef COMPRESSOR_EFFECT
	CreateEffect<CompressorEffect>(m_Effects, &m_ControlManager);
#endif
#ifdef DISTORTION_EFFECT
	CreateEffect<DistortionEffect>(m_Effects, &m_ControlManager); // TODO: Modify the algorithm, so it wouldn't gain the noises probably setting the resonance is the key
#endif
#ifdef NOISE_GATE_EFFECT
	CreateEffect<NoiseGateEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE); // TODO: Tune the Attack and Release time to help Overdrive
#endif
#ifdef OVERDRIVE_EFFECT
	CreateEffect<OverdriveEffect>(m_Effects, &m_ControlManager);
#endif
#ifdef REVERB_EFFECT
	CreateEffect<ReverbEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif
#ifdef SUSTAIN_EFFECT
	CreateEffect<SustainEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif
#ifdef TREMOLO_EFFECT
	CreateEffect<TremoloEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif
#ifdef WAH_EFFECT
	CreateEffect<WahEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE); // TODO: Tune
#endif
#ifdef TEST_EFFECT
	CreateEffect<TestEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
#endif

	// TODO: Tune the values
	// Potentiometer *volumePot = m_ControlManager.CreatePotentiometer(GPIOPins::Pin14);
	// volumePot->SetOnChangedListener(
	// 	[&](float value)
	// 	{
	// 		ESP32A1SCodec::SetMicrophoneGain(Math::Lerp(0.0F, 24, value));
	// 		ESP32A1SCodec::SetInputToMixerGain(Math::Lerp(-15.0F, 6, value));
	// 		ESP32A1SCodec::SetInputVolume(Math::Lerp(-96.0F, 0, value));
	// 		ESP32A1SCodec::SetDigitalVolume(Math::Lerp(-96.0F, 0, value));
	// 		ESP32A1SCodec::SetOutputVolume(Math::Lerp(-45.0F, 4.5F, value));
	// 	});

	ESP32A1SCodec::PrintSystemStatistics();

	Task::Delay(10);

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
	sineWave.SetAmplitude(1);
	sineWave.SetFrequency(NOTE_G5);

	uint32 bufferLen = sineWave.GetBufferLength();
	uint32 sampleCount = bufferLen * 2;

	int32 *outBuffer = Memory::Allocate<int32>(sampleCount);
	double *processBufferL = Memory::Allocate<double>(bufferLen);

	{
		double *inputBufferL = Memory::Allocate<double>(bufferLen);

		for (uint16 i = 0; i < bufferLen; ++i)
			CONVERT_INT32_TO_NORMALIZED_DOUBLE(sineWave.GetBuffer(), false, 0, inputBufferL, i);

		Memory::Copy(inputBufferL, processBufferL, bufferLen);

		for (Effect *effect : m_Effects)
			effect->Apply(processBufferL, bufferLen);

		for (uint16 i = 0; i < bufferLen; ++i)
			printf("%0.10f#%0.10f\n", inputBufferL[i], processBufferL[i]);

		Memory::Deallocate(inputBufferL);
	}

	while (true)
	{
		for (uint16 i = 0; i < bufferLen; ++i)
			CONVERT_INT32_TO_NORMALIZED_DOUBLE(sineWave.GetBuffer(), false, 0, processBufferL, i);

		for (Effect *effect : m_Effects)
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

	int32 *ioBuffer = Memory::Allocate<int32>(SAMPLE_COUNT);
	double *processBufferL = Memory::Allocate<double>(FRAME_LENGTH);

	Task::Delay(10);

	// double sumL = 0;
	// uint16 count = 0;
	// float nextTime = 0;

	while (true)
	{
		ESP32A1SCodec::Read(ioBuffer, SAMPLE_COUNT, 20);

		if (m_Mute)
		{
			Memory::Set(processBufferL, 0, FRAME_LENGTH);
		}
		else
		{
			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				CONVERT_INT32_TO_NORMALIZED_DOUBLE(ioBuffer, true, 0, processBufferL, i);

				// sumL += processBufferL[i];
			}

			// count += FRAME_LENGTH;

			// if (nextTime < Time::Now())
			// {
			// 	nextTime += 1;

			// 	Log::WriteError("Avg: %f", sumL / count);

			// 	sumL = 0;
			// 	count = 0;
			// }
		}

		for (Effect *effect : m_Effects)
			effect->Apply(processBufferL, FRAME_LENGTH);

		for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			SCALE_NORMALIZED_DOUBLE_TO_INT32(processBufferL, i, ioBuffer, true, 0);

		ESP32A1SCodec::Write(ioBuffer, SAMPLE_COUNT, 20);
	}

	Memory::Deallocate(processBufferL);
	Memory::Deallocate(ioBuffer);

	Task::Delete();
}
#endif