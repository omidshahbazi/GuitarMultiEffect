#include "Application.h"
#include "framework/Debug.h"
#include "framework/Memory.h"
#include "framework/ESP32A1SCodec.h"
#include <math.h>
#include <esp_bt.h>
#include <esp_adc_cal.h>

#define FRAME_LENGTH 64
#define SAMPLE_COUNT FRAME_LENGTH / 2
#define FRAME_SIZE FRAME_LENGTH * 4

#define SAMPLE_RATE (44100)
#define WAVE_FREQ_HZ (700)
#define PI (3.14159265)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE / WAVE_FREQ_HZ)

#define SAMPLES SAMPLE_PER_CYCLE // Total number of samples left and right
#define BUF_SAMPLES SAMPLES * 4	 // Size of DMA tx/rx buffer samples * left/right * 2 for 32 bit samples

int16_t txBuf[BUF_SAMPLES];

static void setup_sine_waves16(int amplitude)
{
	double sin_float;

	size_t i2s_bytes_write = 0;

	// printf("\r\nFree mem=%d, written data=%d\n", esp_get_free_heap_size(), BUF_SAMPLES*2 );

	for (int pos = 0; pos < BUF_SAMPLES; pos += 2)
	{
		sin_float = amplitude * sin(pos / 2 * 2 * PI / SAMPLE_PER_CYCLE);

		int lval = sin_float;
		int rval = sin_float;

		txBuf[pos] = lval & 0xFFFF;
		txBuf[pos + 1] = rval & 0xFFFF;

		// printf( "%d  %04x:%04x\n", lval, txBuf[pos],txBuf[pos+1] );
	}
}

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
	configs.Mode = ESP32A1SCodec::Modes::Master;
	configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Both;
	configs.SampleRate = SAMPLE_RATE;
	configs.BitsPerSample = ES8388::BitsPerSamples::BPS24;
	configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
	configs.BufferCount = 3;
	configs.BufferLegth = 300;
	configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
	configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

	CHECK_CALL(ESP32A1SCodec::Initialize(&configs));
	// CHECK_CALL(ESP32A1SCodec::SetOutputVolume(70));
	// CHECK_CALL(ESP32A1SCodec::SetMicrophoneGain(8));

	xTaskCreatePinnedToCore(I2SRoutine, "i2s_task", 4096, this, 10, nullptr, 1);

	// #define ES8388_SDA (GPIO_NUM_18)
	// #define ES8388_SCK (GPIO_NUM_23)
	// #define ES8388_ADDR 0x10

	// 	codecBusInit(ES8388_SDA, ES8388_SCK, 32000);
	// 	ES8388Codec codec;
	// 	codec.setInputMode(0);
	// 	codec.init(ES8388_ADDR);
}

void Application::Update(void)
{
}

void Application::I2SRoutine(void)
{
#if 0
	while (true)
	{
		ESP32A1SCodec::Read(m_InBufferInt, FRAME_LENGTH, 20);

		ESP32A1SCodec::Write(m_InBufferInt, FRAME_LENGTH);

		// const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
		// vTaskDelay(xDelay);
	}
#else
	int amplitude = 8000;
	int start_dir = 50;
	int dir = start_dir;

	while (true)
	{

		amplitude -= dir;
		if (amplitude <= start_dir || amplitude >= 15000)
			dir *= -1;

		setup_sine_waves16(amplitude);

		ESP32A1SCodec::Write(txBuf, BUF_SAMPLES);

		// const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
		// vTaskDelay(xDelay);
	}
#endif

	// vTaskDelay(1000);
	vTaskDelete(nullptr);
}