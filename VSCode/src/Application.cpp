
#if 1

#include "Application.h"
#include "framework/include/Debug.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"

#define FRAME_LENGTH 64
#define FULL_24_BITS 0xFFFFFF

Application::Application(void)
	: m_Mute(false),
	  m_OutCorrectionGain(1),
	  m_InBufferInt(nullptr),
	  m_InBuffer(nullptr),
	  m_OutBufferInt(nullptr),
	  m_OutBuffer(nullptr)
{
	Log::SetMask(Log::Types::General);

	m_InBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);
	m_InBuffer = Memory::Allocate<float>(FRAME_LENGTH);
	m_OutBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);
	m_OutBuffer = Memory::Allocate<float>(FRAME_LENGTH);
}

void Application::Initialize(void)
{
	ESP32A1SCodec::Configs configs;
	configs.Version = ESP32A1SCodec::Versions::V2974;
	configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Both;
	configs.SampleRate = 44100;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS32;
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

		if (m_Mute)
			Memory::Set(m_InBuffer, 0.0F, FRAME_LENGTH);
		else
			for (int i = 0; i < FRAME_LENGTH; ++i)
			{
				// convert to 24 bit int then to float
				m_InBuffer[i] = (float)(m_InBufferInt[i] >> 8);

				// scale to 1.0
				m_InBuffer[i] = m_InBuffer[i] / FULL_24_BITS;
			}

		for (int i = 0; i < FRAME_LENGTH; ++i)
		{
			m_OutBuffer[i] = m_InBuffer[i];
		}

		// convert back float to int
		for (int i = 0; i < FRAME_LENGTH; ++i)
		{
			// scale the left output to 24 bit range
			m_OutBuffer[i] = m_OutCorrectionGain * m_OutBuffer[i] * FULL_24_BITS;

			// saturate to signed 24bit range
			m_OutBuffer[i] = Math::Clamp(m_OutBuffer[i], -FULL_24_BITS, FULL_24_BITS);

			m_OutBufferInt[i] = ((int32)m_OutBuffer[i]) << 8;
		}

		ESP32A1SCodec::Write(m_OutBufferInt, FRAME_LENGTH);
	}

	vTaskDelete(nullptr);
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

void Application::Update(void)
{
}

void Application::I2SRoutine(void)
{
}

#endif