#ifdef AUTO_WAH_EFFECT

#include "Effects/AutoWahEffect.h"
#include "ControlManager.h"
#include <framework/include/Controls/Potentiometer.h>
#include <framework/include/Controls/Potentiometer.h>

AutoWahEffect::AutoWahEffect(ControlManager *ControlManager, uint32 SampleRate)
	: Effect(ControlManager),
	  m_AutoWah(SampleRate)
{
}

IDSP *AutoWahEffect::GetDSP(void)
{
	return reinterpret_cast<IDSP *>(&m_AutoWah);
}

#endif