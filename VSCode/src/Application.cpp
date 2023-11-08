
#if 1

#include "Application.h"
#include "DelayEffect.h"
#include "WahWahEffect.h"
#include "framework/include/Memory.h"
#include "framework/include/ESP32A1SCodec.h"
#include <Arduino.h>

#define FRAME_LENGTH 64
// #define FULL_24_BITS 0xFFFFFF
#define FULL_24_BITS 8388607

template <typename T>
void CreateEffect(Application::EffectList &Effects)
{
	T *effect = Memory::Allocate<T>();

	new (effect) T();

	Effects.push_back(effect);
}

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

	CreateEffect<DelayEffect>(m_Effects);
	CreateEffect<WahWahEffect>(m_Effects);

	Serial.begin(115200);

	xTaskCreatePinnedToCore(PassthroughTask, "PassthroughTask", 4096, this, 10, nullptr, 1);
}

void Application::Update(void)
{
}

void Application::PassthroughTask(void)
{
	int32 *inBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);
	float *processedBuffer = Memory::Allocate<float>(FRAME_LENGTH);
	int32 *outBufferInt = Memory::Allocate<int32>(FRAME_LENGTH);

	while (true)
	{
		ESP32A1SCodec::Read(inBufferInt, FRAME_LENGTH, 20);

		if (m_Mute)
			Memory::Set(processedBuffer, 0.0F, FRAME_LENGTH);
		else
			for (uint16 i = 0; i < FRAME_LENGTH; ++i)
			{
				// convert to 24 bit int then to float
				processedBuffer[i] = (float)(inBufferInt[i] >> 8);

				// scale to 1.0
				processedBuffer[i] /= FULL_24_BITS;
			}

		for (IEffect *effect : m_Effects)
			effect->Process(processedBuffer, FRAME_LENGTH);

		// Serial.print("\n Input");
		// for (int i = 0; i < FRAME_LENGTH; ++i)
		// 	Serial.printf("%f, ", inBuffer[i]);
		// Serial.print("\n");
		// Serial.print("\n Processed");
		// for (int i = 0; i < FRAME_LENGTH; ++i)
		// 	Serial.printf("%f, ", processedBuffer[i]);
		// Serial.print("\n");

		// Serial.print("\n Output");

		// convert back float to int
		for (uint16 i = 0; i < FRAME_LENGTH; ++i)
		{
			// scale the left output to 24 bit range
			processedBuffer[i] = m_OutCorrectionGain * processedBuffer[i] * FULL_24_BITS;

			// saturate to signed 24bit range
			processedBuffer[i] = Math::Clamp(processedBuffer[i], -FULL_24_BITS, FULL_24_BITS);

			outBufferInt[i] = ((int32)processedBuffer[i]) << 8;

			// Serial.printf("{%i,%i} ", inBufferInt[i], outBufferInt[i]);
		}

		// Serial.print("\n");

		ESP32A1SCodec::Write(outBufferInt, FRAME_LENGTH);
	}

	Memory::Deallocate(outBufferInt);
	Memory::Deallocate(processedBuffer);
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