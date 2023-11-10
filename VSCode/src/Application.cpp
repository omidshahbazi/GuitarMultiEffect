#if 1

#include "Application.h"
#include "DelayEffect.h"
#include "WahWahEffect.h"
#include "OverdriveEffect.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include "framework/include/Task.h"

#include <sstream>

const uint32 SAMPLE_RATE = 44100;
const uint16 FRAME_LENGTH = 32;
const int32 FULL_24_BITS = 0x7FFFFF;

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
	configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Both;
	configs.SampleRate = SAMPLE_RATE;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS32;
	configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
	configs.BufferCount = 3;
	configs.BufferLegth = 300;
	configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
	configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

	ESP32A1SCodec::Initialize(&configs);
	ESP32A1SCodec::OptimizeConversion(2);

	// CreateEffect<DelayEffect>(m_Effects, FRAME_LENGTH, SAMPLE_RATE);
	CreateEffect<WahWahEffect>(m_Effects, SAMPLE_RATE);
	//  CreateEffect<OverdriveEffect>(m_Effects);

	Task::Create(
		[this]()
		{
			this->PassthroughTask();
		},
		1, 10);
}

void Application::PassthroughTask(void)
{
	Task::Delay(1000);

	Log::WriteInfo("Starting Passthrough Task");

	int32 *ioBuffer = Memory::Allocate<int32>(FRAME_LENGTH);
	double *processBuffer = Memory::Allocate<double>(FRAME_LENGTH);

	while (true)
	{
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

			// scale the left output to 24 bit range
			process = m_OutCorrectionGain * process * FULL_24_BITS;

			// saturate to signed 24bit range
			process = Math::Clamp(process, -FULL_24_BITS, FULL_24_BITS);

			ioBuffer[i] = ((int32)process) << 8;

			// Log::WriteInfo("Output %f,%i", processBuffer[i], ioBuffer[i]);
		}

		ESP32A1SCodec::Write(ioBuffer, FRAME_LENGTH);
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