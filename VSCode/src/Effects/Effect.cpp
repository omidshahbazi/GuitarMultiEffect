#include "Effects/Effect.h"
#include "framework/include/DSP/IDSP.h"

Effect::Effect(void)
	: m_Enabled(true)
{
}

void Effect::Apply(double *Buffer, uint16 Count)
{
	if (!m_Enabled)
		return;

	ProcessBuffer(Buffer, Count);
}

void Effect::ToggleEnabled(void)
{
	m_Enabled = !m_Enabled;
}

void Effect::ProcessBuffer(double *Buffer, uint16 Count)
{
	GetDSP()->ProcessBuffer(Buffer, Count);
}