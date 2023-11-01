
#if 0

#include "Application.h"
#include "framework/include/Debug.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"

#define FRAME_LENGTH 64
#define FRAME_SIZE FRAME_LENGTH * 4

Application::Application(void)
	: m_InBufferInt(nullptr),
	  m_InBuffer(nullptr),
	  m_OutBufferInt(nullptr),
	  m_OutBuffer(nullptr)
{
	Log::SetMask(Log::Types::General);

	m_InBufferInt = Allocate<int32>(FRAME_LENGTH);
	m_InBuffer = Allocate<float>(FRAME_SIZE);
	m_OutBufferInt = Allocate<int32>(FRAME_LENGTH);
	m_OutBuffer = Allocate<float>(FRAME_SIZE);
}

void Application::Initialize(void)
{
	ESP32A1SCodec::Configs configs;
	configs.Version = ESP32A1SCodec::Versions::V2974;
	configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Both;
	configs.SampleRate = 44100;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS16;
	configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
	configs.BufferCount = 3;
	configs.BufferLegth = 300;
	configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
	configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

	CHECK_CALL(ESP32A1SCodec::Initialize(&configs));
	// CHECK_CALL(ESP32A1SCodec::SetOutputVolume(70));
	// CHECK_CALL(ESP32A1SCodec::SetMicrophoneGain(8));

	xTaskCreatePinnedToCore(I2SRoutine, "I2SRoutine", 4096, this, 10, nullptr, 1);
}

void Application::Update(void)
{
}

void Application::I2SRoutine(void)
{
	while (true)
	{
		ESP32A1SCodec::Read(m_InBufferInt, FRAME_LENGTH, 20);

		ESP32A1SCodec::Write(m_InBufferInt, FRAME_LENGTH);
	}

	vTaskDelete(nullptr);
}

#else

#include "Application.h"
#include "framework/examples/SineWavePlayer.h"

Application::Application(void)
{
}

void Application::Initialize(void)
{
	SineWavePlayer::EntryPoint();
}

void Application::Update(void)
{
}

void Application::I2SRoutine(void)
{
}

#endif