#include "Application.h"
#include <memory.h>

#define FRAME_LENGTH 64
#define SAMPLE_COUNT FRAME_LENGTH / 2
#define FRAME_SIZE FRAME_LENGTH * 4

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
}

void Application::I2SRoutine(void)
{
}