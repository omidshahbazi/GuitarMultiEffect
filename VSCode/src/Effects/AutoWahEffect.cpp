#include "Effects/AutoWahEffect.h"
#include "ControlManager.h"
#include "framework/include/Controls/Potentiometer.h"

AutoWahEffect::AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate)
{
}

IDSP *AutoWahEffect::GetDSP(void)
{
	return &m_AutoWah;
}