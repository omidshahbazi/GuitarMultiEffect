#pragma once
#pragma GCC push_options
#pragma GCC optimize("Os")
#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "Preset.h"
#include "../framework/DSP/PersistentBlob.h"
#include <string>

class PresetManager
{
public:
	static const uint8 PRESET_COUNT = 4;

private:
	struct Data
	{
	public:
		Preset::Data Data[PRESET_COUNT];
	};

public:
	PresetManager(void)
		: m_CurrentPresetIndex(0),
		  m_PersistentData(0)
	{
	}

	void Initialize(void)
	{
		static Data defaultData = {};
		for (uint8 i = 0; i < PRESET_COUNT; ++i)
		{
			Preset::Data &data = defaultData.Data[i];

			Preset::SetName(data, ("PRESET " + std::to_string(i + 1)).c_str());
			data.Volume = 1;

			data.OverdriveData.Rate = 1;
			data.OverdriveData.Gain = 0.5;
		}

		m_PersistentData.Initialize(defaultData);
		const Data &data = m_PersistentData.Get();

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
		{
			Preset &preset = m_Presets[i];

			new (&preset) Preset();

			preset.SetData(data.Data[i]);
		}
	}

	Preset *GetCurrentPreset(void)
	{
		return &m_Presets[m_CurrentPresetIndex];
	}

	uint8 GetCurrentPresetIndex(void)
	{
		return m_CurrentPresetIndex;
	}

	void Save(void)
	{
		Data data;
		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			data.Data[i] = m_Presets[i].GetData();

		m_PersistentData.Set(data);

		PersistentBlobBase::SaveAll();
	}

	void Ereas(void)
	{
		PersistentBlobBase::EreasAll();

		// TODO: what else?
	}

private:
	Preset m_Presets[PRESET_COUNT];
	uint8 m_CurrentPresetIndex;

	PersistentBlob<Data> m_PersistentData;
};

#endif
#pragma GCC pop_options