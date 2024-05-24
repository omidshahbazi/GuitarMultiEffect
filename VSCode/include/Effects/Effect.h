#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef EFFECT_H
#define EFFECT_H

#include "../Common.h"

class Effect
{
public:
	struct Data
	{
	public:
		uint8 Index;
		bool Enabled;
	};

public:
	Effect(void)
		: m_Enabled(true)
	{
	}

	void Apply(SampleType *Buffer, uint8 Count)
	{
		if (!m_Enabled)
			return;

		Process(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		m_Enabled = Data.Enabled;
	}

protected:
	virtual void Process(SampleType *Buffer, uint8 Count) = 0;

private:
	bool m_Enabled;
};

#endif
#pragma GCC pop_options