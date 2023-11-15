#if 1

#include "Application.h"
#include "DelayEffect.h"
#include "WahWahEffect.h"
#include "OverdriveEffect.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include "framework/include/Task.h"
#include "framework/include/Time.h"
#include "framework/include/PushButtonArray.h"
#include "framework/include/BufferUtils.h"

const uint32 SAMPLE_RATE = 44100;
const uint16 SAMPLE_COUNT = 64;
const uint16 FRAME_LENGTH = SAMPLE_COUNT / 2;

PushButtonArray buttons(GPIOPins::Pin14, 2);

template <typename T, typename... ArgsT>
T *CreateEffect(Application::EffectList &Effects, ArgsT... Args)
{
	T *effect = Memory::Allocate<T>();

	new (effect) T(Args...);

	Effects.push_back(effect);

	return effect;
}

Application::Application(void)
	: m_Mute(false)
{
	Log::SetMask(Log::Types::General);
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
	configs.InputMode = ESP32A1SCodec::InputModes::LineL;
	configs.OutputMode = ESP32A1SCodec::OutputModes::SpeakerL;
	configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
	configs.EnableNoiseGate = true;
	configs.EnableAutomaticLevelControl = false;

	ESP32A1SCodec::Initialize(&configs);
	// ESP32A1SCodec::OptimizeConversion(4);
	// ESP32A1SCodec::SetMicrophoneGain(24);
	ESP32A1SCodec::SetOutputVolume(0);

	// CreateEffect<DelayEffect>(m_Effects, FRAME_LENGTH, SAMPLE_RATE);
	// CreateEffect<WahWahEffect>(m_Effects, SAMPLE_RATE);
	// CreateEffect<OverdriveEffect>(m_Effects);

	Task::Create(
		[this]()
		{
			this->PassthroughTask();
		},
		1, 10);

	buttons.Bind(
		1,
		[]()
		{
			Log::WriteError("1 Pressed");
		},
		nullptr, nullptr);

	buttons.Bind(
		0,
		nullptr,
		[]()
		{
			Log::WriteError("0 Hold");
		},
		nullptr);
}

void Application::PassthroughTask(void)
{
	Log::WriteInfo("Starting Passthrough Task");

	int32 *ioBuffer = Memory::Allocate<int32>(SAMPLE_COUNT);
	double *processBufferL = Memory::Allocate<double>(FRAME_LENGTH);
	double *processBufferR = Memory::Allocate<double>(FRAME_LENGTH);

	double sumL = 0, sumR = 0;
	uint16 count = 0;
	float nextTime = 0;

	while (true)
	{
		buttons.Update();

		ESP32A1SCodec::Read(ioBuffer, SAMPLE_COUNT, 20);

		if (m_Mute)
		{
			Memory::Set(processBufferL, 0, FRAME_LENGTH);
			// Memory::Set(processBufferR, 0, FRAME_LENGTH);
		}
		else
		{
			// for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			// 	CONVERT_TO_24_AND_NORMALIZED_DOUBLE(processBufferL, i, ioBuffer, 0);

			// for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			// 	CONVERT_TO_24_AND_NORMALIZED_DOUBLE(processBufferR, i, ioBuffer, 1);

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				CONVERT_TO_24_AND_NORMALIZED_DOUBLE(processBufferL, i, ioBuffer, 0);

				sumL += processBufferL[i];
			}

			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				CONVERT_TO_24_AND_NORMALIZED_DOUBLE(processBufferR, i, ioBuffer, 1);

				sumR += processBufferR[i];
			}

			count += FRAME_LENGTH;

			if (nextTime < Time::Now())
			{
				nextTime += 1;

				Log::WriteError("Avg L: %f, R: %f", sumL / count, sumR / count);

				sumL = sumR = 0;
				count = 0;
			}
		}

		for (Effect *effect : m_Effects)
			effect->Apply(processBufferL, FRAME_LENGTH);

		for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			SCALE_TO_24_AND_SATURATED_32(processBufferL, i, ioBuffer, 0);

		for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			SCALE_TO_24_AND_SATURATED_32(processBufferR, i, ioBuffer, 1);

		ESP32A1SCodec::Write(ioBuffer, SAMPLE_COUNT);
	}

	Memory::Deallocate(processBufferL);
	// Memory::Deallocate(processBufferR);
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