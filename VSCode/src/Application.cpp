#include "Application.h"
#include "framework/Debug.h"
#include "framework/Memory.h"
#include "framework/ESP32A1SAudioModule.h"

#define FRAME_LENGTH 64
#define SAMPLE_COUNT FRAME_LENGTH / 2
#define FRAME_SIZE FRAME_LENGTH * 4

Application::Application(void)
	: m_InBufferInt(nullptr),
	  m_InBuffer(nullptr),
	  m_OutBufferInt(nullptr),
	  m_OutBuffer(nullptr)
{
	m_InBufferInt = Allocate<int32>(FRAME_LENGTH);
	m_InBuffer = Allocate<float>(FRAME_SIZE);
	m_OutBufferInt = Allocate<int32>(FRAME_LENGTH);
	m_OutBuffer = Allocate<float>(FRAME_SIZE);
}

void Application::Initialize(void)
{
	ESP32A1SAudioModule::Configs configs;
	configs.Version = ESP32A1SAudioModule::Versions::V2974;
	configs.Mode = ESP32A1SAudioModule::Modes::Master;
	configs.TransmissionMode = ESP32A1SAudioModule::TransmissionModes::Both;
	configs.SampleRate = 44100;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS16;
	configs.ChannelFormat = ESP32A1SAudioModule::ChannelFormats::SeparatedLeftAndRight;
	configs.BufferCount = 3;
	configs.BufferLegth = 300;
	configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
	configs.OutputMode = ES8388::OutputModes::AllLineOutputs;
	configs.Format = ES8388::Formats::Normal;

	CHECK_CALL(ESP32A1SAudioModule::Initialize(&configs));

	xTaskCreatePinnedToCore(I2SRoutine, "i2s_task", 4096, this, 10, nullptr, 1);
}

void Application::Update(void)
{
}

void Application::I2SRoutine(void)
{
	vTaskDelay(1000);

	uint16 data = 10000;

	while (true)
	{
		ESP32A1SAudioModule::Write(&data, 1);
	}

	vTaskDelete(nullptr);
}