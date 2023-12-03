#if 1

#include "Application.h"
#include "framework/include/Time.h"
#include "framework/include/Task.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include "framework/include/BufferUtils.h"
#include "Effects/OverdriveEffect.h"
#include "Effects/WahEffect.h"
#include "Effects/AutoWahEffect.h"
#include "Effects/TremoloEffect.h"
#include "Effects/NoiseGateEffect.h"
#include "Effects/ReverbEffect.h"
#include "Effects/ChorusEffect.h"
#include "Effects/TestEffect.h"

const uint16 SAMPLE_RATE = SAMPLE_RATE_15750;
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
	configs.OutputMode = ESP32A1SCodec::OutputModes::SpeakerL;
	configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
	configs.EnableNoiseGate = true;
	configs.EnableAutomaticLevelControl = false;

	ESP32A1SCodec::Initialize(&configs);

	// CreateEffect<OverdriveEffect>(m_Effects, &m_ControlManager);
	// CreateEffect<TremoloEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
	// CreateEffect<ReverbEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
	// CreateEffect<ChorusEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);

	CreateEffect<WahEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
	// CreateEffect<NoiseGateEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);
	// CreateEffect<AutoWahEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);

	// CreateEffect<TestEffect>(m_Effects, &m_ControlManager, SAMPLE_RATE);

	// TODO: Look for other effects
	// TODO: Tune NoiseGateEfect Attack and Release
	// TODO: Test NoiseGateEfect with Overdrive
	// TODO: Tune ChorusEffect
	// TODO: Fix Wah and AutoWah
	// Sustain
	// Phaser
	// Compressor
	// Distortion

	ESP32A1SCodec::PrintSystemStatistics();

	Task::Delay(10);

	Task::Create(
		[this]()
		{
			PassthroughTask();
		},
		4096, "PassthroughTask", 1, 10);

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
}

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
	: m_Mute(false),
	  m_ControlManager(GPIOPins::Pin13)
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