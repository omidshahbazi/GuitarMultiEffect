#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "../Common.h"

class Effect
{
public:
	struct Data
	{
	public:
		Data(void)
			: Index(0),
			  Enabled(false)
		{
		}

		uint8 Index;
		bool Enabled;
	};

public:
	Effect(void)
	{
	}

	void Apply(SampleType *Buffer, uint8 Count)
	{
		if (!m_IsEnabled)
			return;

		Process(Buffer, Count);
	}

	void SetData(const Data &Data)
	{
		m_IsEnabled = Data.Enabled;
	}

	bool GetIsEnabled(void) const
	{
		return m_IsEnabled;
	}

	virtual cstr GetName(void) const = 0;

protected:
	virtual void Process(SampleType *Buffer, uint8 Count) = 0;

private:
	bool m_IsEnabled;
};

#endif