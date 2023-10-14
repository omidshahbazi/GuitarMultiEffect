#include "Application.h"
#include "Arduino.h"
#include "driver/i2s.h"
#include "esp_task_wdt.h"

#define SAMPLE_RATE 16000 // 44100
#define DMA_BUFFER_COUNT 2
#define DMA_BUFFER_LENGTH 32

// audio processing frame length in samples (L+R) 64 samples (32R+32L) 256 Bytes
#define FRAME_LENGTH 64
// sample count per channel for each frame (32)re
#define SAMPLE_COUNT FRAME_LENGTH / 2
// frame size in bytes
#define FRAME_SIZE FRAME_LENGTH * 4
// audio processing priority
#define AUDIO_PROCESS_PRIORITY 10

#define I2S_NUM 0

#define I2S_BCK_IO GPIO_NUM_5
#define I2S_WS_IO GPIO_NUM_25
#define I2S_DI_IO GPIO_NUM_35
#define I2S_DO_IO GPIO_NUM_26

Application::Application(void)
{
	m_InBufferInt = reinterpret_cast<int32_t *>(malloc(FRAME_LENGTH));
	m_InBuffer = reinterpret_cast<float *>(malloc(FRAME_SIZE));

	m_OutBufferInt = reinterpret_cast<int32_t *>(malloc(FRAME_LENGTH));
	memset(m_OutBufferInt, 0, FRAME_LENGTH);

	m_OutBuffer = reinterpret_cast<float *>(malloc(FRAME_SIZE));
	memset(m_OutBuffer, 0, FRAME_SIZE);

	m_OutCorrectionGain = 1;
}

void Application::Initialize(void)
{
	SetupI2S();
}

void Application::Update(void)
{
}

void Application::SetupI2S(void)
{
	i2s_config_t i2s_config;
	{
		i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX);
		i2s_config.sample_rate = SAMPLE_RATE;
		i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
		i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
		i2s_config.communication_format = I2S_COMM_FORMAT_STAND_I2S;

		i2s_config.dma_buf_count = DMA_BUFFER_COUNT;
		i2s_config.dma_buf_len = DMA_BUFFER_LENGTH;

		i2s_config.use_apll = false;
		i2s_config.fixed_mclk = 0;

		i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
	}
	i2s_driver_install((i2s_port_t)I2S_NUM, &i2s_config, 0, nullptr);

	i2s_pin_config_t pin_config;
	{
		pin_config.bck_io_num = I2S_BCK_IO;
		pin_config.ws_io_num = I2S_WS_IO;
		pin_config.data_out_num = I2S_DO_IO;
		pin_config.data_in_num = I2S_DI_IO;
	}
	i2s_set_pin((i2s_port_t)I2S_NUM, &pin_config);

	i2s_set_clk((i2s_port_t)I2S_NUM, SAMPLE_RATE, (i2s_bits_per_sample_t)32, I2S_CHANNEL_STEREO);

	xTaskCreatePinnedToCore(I2SRoutine, "i2s_task", 4096, this, AUDIO_PROCESS_PRIORITY, nullptr, 1);
}

void Application::I2SRoutine(void)
{
	// silence the signal during the first 1000 ms startup
	vTaskDelay(1000);

	size_t bytesread, byteswritten;

	m_UsedTicksStart = xthal_get_ccount();
	m_AvailableTicksStart = xthal_get_ccount();

	while (true)
	{
		m_AvailableTicksEnd = xthal_get_ccount();
		m_AvailableTicks = m_AvailableTicksEnd - m_AvailableTicksStart;
		m_AvailableTicksStart = m_AvailableTicksEnd;

		i2s_read((i2s_port_t)I2S_NUM, (void *)m_InBufferInt, FRAME_SIZE, &bytesread, 20);

		// used-tick counter starting point
		m_UsedTicksStart = xthal_get_ccount();

		for (int i = 0; i < SAMPLE_COUNT; ++i)
		{
			if (!m_IsMuted)
			{
				// convert to 24 bit int then to float
				m_InBuffer[i] = (float)(m_InBufferInt[i] >> 8);

				// scale to 1.0
				m_InBuffer[i] /= 8388608;
			}
			else
				m_InBuffer[i] = 0;
		}

		// process the signal by the effect module
		//_module->process(m_InBuffer, m_OutBuffer, SAMPLE_COUNT);

		// convert back float to int
		for (int i = 0; i < SAMPLE_COUNT; i++)
		{
			// scale the left output to 24 bit range
			m_OutBuffer[i] = m_OutCorrectionGain * m_OutBuffer[i] * 8388607;
			// saturate to signed 24bit range
			if (m_OutBuffer[i] > 8388607)
				m_OutBuffer[i] = 8388607;

			if (m_OutBuffer[i] < -8388607)
				m_OutBuffer[i] = -8388607;

			m_OutBufferInt[i] = ((int32_t)m_OutBuffer[i]) << 8;
		}

		// used-tick counter end point
		m_UsedTicksEnd = xthal_get_ccount();
		m_UsedTicks = m_UsedTicksEnd - m_UsedTicksStart;

		i2s_write((i2s_port_t)I2S_NUM, (void *)m_OutBufferInt, FRAME_SIZE, &byteswritten, 20);

		esp_task_wdt_reset();
	}

	vTaskDelete(nullptr);
}