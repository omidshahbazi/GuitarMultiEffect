
#if 1

#include "Application.h"
#include "Delay.h"
#include "framework/include/Debug.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"

#define FRAME_LENGTH 64
#define FULL_24_BITS 0xFFFFFF

Application::Application(void)
	: m_Mute(false),
	  m_OutCorrectionGain(1)
{
	Log::SetMask(Log::Types::General);
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

	xTaskCreatePinnedToCore(PassthroughTask, "PassthroughTask", 4096, this, 10, nullptr, 1);
}

void Application::Update(void)
{
}

void Application::PassthroughTask(void)
{
	int32 *inBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);
	float *inBuffer = Memory::Allocate<float>(FRAME_LENGTH);
	float *processedBuffer = Memory::Allocate<float>(FRAME_LENGTH);
	int32 *outBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);

	Delay delay;

	while (true)
	{
		ESP32A1SCodec::Read(inBufferInt, FRAME_LENGTH, 20);

		if (m_Mute)
			Memory::Set(inBuffer, 0.0F, FRAME_LENGTH);
		else
			for (int i = 0; i < FRAME_LENGTH; ++i)
			{
				// convert to 24 bit int then to float
				inBuffer[i] = (float)(inBufferInt[i] >> 8);

				// scale to 1.0
				inBuffer[i] = inBuffer[i] / FULL_24_BITS;
			}

		Memory::Copy(inBuffer, processedBuffer, FRAME_LENGTH);

		// Effects
		delay.Process(inBuffer, processedBuffer, FRAME_LENGTH);

		// convert back float to int
		for (int i = 0; i < FRAME_LENGTH; ++i)
		{
			// scale the left output to 24 bit range
			processedBuffer[i] = m_OutCorrectionGain * processedBuffer[i] * FULL_24_BITS;

			// saturate to signed 24bit range
			processedBuffer[i] = Math::Clamp(processedBuffer[i], -FULL_24_BITS, FULL_24_BITS);

			outBufferInt[i] = ((int32)processedBuffer[i]) << 8;
		}

		ESP32A1SCodec::Write(outBufferInt, FRAME_LENGTH);
	}

	Memory::Deallocate(outBufferInt);
	Memory::Deallocate(processedBuffer);
	Memory::Deallocate(inBuffer);
	Memory::Deallocate(inBufferInt);

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