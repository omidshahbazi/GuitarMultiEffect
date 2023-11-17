#if 1

#include "Application.h"
#include "Effects/TestEffect.h"
#include "Effects/OverdriveEffect.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include "framework/include/Task.h"
#include "framework/include/Time.h"
#include "framework/include/BufferUtils.h"

const uint32 SAMPLE_RATE = 44100;
const uint16 SAMPLE_COUNT = 64;
const uint16 FRAME_LENGTH = SAMPLE_COUNT / 2;

template <typename T, typename... ArgsT>
T *CreateEffect(Application::EffectList &Effects, ArgsT... Args)
{
	T *effect = Memory::Allocate<T>();

	new (effect) T(Args...);

	Effects.push_back(effect);

	return effect;
}

Application::Application(void)
	: m_Mute(false),
	  m_ControlManager(GPIOPins::Pin13)
{
	Time::Initialize();
	Log::SetMask(Log::Types::General);
}

void Application::Initialize(void)
{
	Log::WriteInfo("Initializing");

	ESP32A1SCodec::Configs configs;
	configs.Version = ESP32A1SCodec::Versions::V2974;
	configs.SampleRate = SAMPLE_RATE;
	configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS32;
	configs.BufferCount = 3;
	configs.BufferLength = 300;
	configs.InputMode = ESP32A1SCodec::InputModes::LineL;
	configs.OutputMode = ESP32A1SCodec::OutputModes::SpeakerL;
	configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
	configs.EnableNoiseGate = true;
	configs.EnableAutomaticLevelControl = false;

	ESP32A1SCodec::Initialize(&configs);
	// ESP32A1SCodec::OptimizeConversion(4);
	// ESP32A1SCodec::SetMicrophoneGain(24);
	ESP32A1SCodec::SetOutputVolume(0);

	CreateEffect<TestEffect>(m_Effects, &m_ControlManager);
	// CreateEffect<OverdriveEffect>(m_Effects, &m_ControlManager);

	Task::Create(
		[&]()
		{
			PassthroughTask();
		},
		1, 10);
}

void Application::PassthroughTask(void)
{
	Log::WriteInfo("Starting Passthrough Task");

	int32 *ioBuffer = Memory::Allocate<int32>(SAMPLE_COUNT);
	double *processBufferL = Memory::Allocate<double>(FRAME_LENGTH);

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
				CONVERT_TO_24_AND_NORMALIZED_DOUBLE(processBufferL, i, ioBuffer, 0);

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
			SCALE_TO_24_AND_SATURATED_32(processBufferL, i, ioBuffer, 0);

		ESP32A1SCodec::Write(ioBuffer, SAMPLE_COUNT, 20);
	}

	Memory::Deallocate(processBufferL);
	Memory::Deallocate(ioBuffer);

	Task::Delete();
}

#else

#include "Application.h"
#include "framework/examples/SineWavePlayer.h"
// #include "framework/examples/BluetoothPlayer.h"

Application::Application(void)
{
}

void Application::Initialize(void)
{
	SineWavePlayer::EntryPoint();
	// BluetoothPlayer::EntryPoint();
}

void Application::PassthroughTask(void)
{
}

#endif