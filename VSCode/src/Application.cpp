#include "Application.h"
#include <memory.h>
#include "framework/Debug.h"
#include "framework/Memory.h"
#include "framework/AnalogReadStream.h"

#define FRAME_LENGTH 64
#define SAMPLE_COUNT FRAME_LENGTH / 2
#define FRAME_SIZE FRAME_LENGTH * 4

Application::Application(void)
	: m_ReadStream(nullptr),
	  m_InBufferInt(nullptr),
	  m_InBuffer(nullptr),
	  m_OutBufferInt(nullptr),
	  m_OutBuffer(nullptr)
{
	m_ReadStream = Allocate<AnalogReadStream>();
	m_InBufferInt = Allocate<int32_t>(FRAME_LENGTH);
	m_InBuffer = Allocate<float>(FRAME_SIZE);
	m_OutBufferInt = Allocate<int32_t>(FRAME_LENGTH);
	m_OutBuffer = Allocate<float>(FRAME_SIZE);
}

void Application::Initialize(void)
{
	CHECK_CALL(m_ReadStream->Init());

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