#if 1

// #define PRINT_SAMPLES_AVERAGE

#include "Application.h"
#include "DelayEffect.h"
#include "WahWahEffect.h"
#include "OverdriveEffect.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include "framework/include/Task.h"
#include "framework/include/Time.h"
#include "framework/include/PushButtonArray.h"

const uint32 SAMPLE_RATE = 44100;
const uint16 FRAME_LENGTH = 32;
const int32 FULL_24_BITS = 0x7FFFFF;

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
	: m_Mute(false),
	  m_OutCorrectionGain(1)
{
	Log::SetMask(Log::Types::General);
}

void Application::Initialize(void)
{
	Log::WriteInfo("Initializing");

	ESP32A1SCodec::Configs configs;
	configs.Version = ESP32A1SCodec::Versions::V2974;
	configs.SampleRate = SAMPLE_RATE;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS32;
	configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
	configs.BufferCount = 3;
	configs.BufferLegth = 300;
	configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
	configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

	ESP32A1SCodec::Initialize(&configs);
	// ESP32A1SCodec::OptimizeConversion(4);
	ESP32A1SCodec::SetMicrophoneGain(24);
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
	Task::Delay(1000);

	Log::WriteInfo("Starting Passthrough Task");

	int32 *ioBuffer = Memory::Allocate<int32>(FRAME_LENGTH);
	double *processBuffer = Memory::Allocate<double>(FRAME_LENGTH);

#ifdef PRINT_SAMPLES_AVERAGE
	double sum = 0;
	uint16 count = 0;
	double nextTime = Time::Now();
#endif

	while (true)
	{
		// buttons.Update();

		ESP32A1SCodec::Read(ioBuffer, FRAME_LENGTH, 20);

		if (m_Mute)
			Memory::Set(processBuffer, 0, FRAME_LENGTH);
		else
			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				// convert to 24 bit int then to float
				processBuffer[i] = (double)(ioBuffer[i] >> 8);

				// scale to 1.0
				processBuffer[i] /= FULL_24_BITS;
			}

		for (Effect *effect : m_Effects)
			effect->Apply(processBuffer, FRAME_LENGTH);

		// convert back float to int
		for (uint16 i = 0; i < FRAME_LENGTH; ++i)
		{
			double process = processBuffer[i];

#ifdef PRINT_SAMPLES_AVERAGE
			sum += process;
			++count;
#endif

			// scale the left output to 24 bit range
			process = m_OutCorrectionGain * process * FULL_24_BITS;

			// saturate to signed 24bit range
			process = Math::Clamp(process, -FULL_24_BITS, FULL_24_BITS);

			ioBuffer[i] = ((int32)process) << 8;

			// Log::WriteInfo("Output %f,%i", processBuffer[i], ioBuffer[i]);
		}

		ESP32A1SCodec::Write(ioBuffer, FRAME_LENGTH);

#ifdef PRINT_SAMPLES_AVERAGE
		if (nextTime < Time::Now())
		{
			Log::WriteInfo("Sample Average %f,%f,%i", sum / count, sum, count);

			sum = 0;
			count = 0;
			nextTime += 1;
		}
#endif
	}

	Memory::Deallocate(processBuffer);
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