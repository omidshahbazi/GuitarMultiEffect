#pragma once
#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "Preset.h"
#include "../framework/DSP/PersistentBlob.h"
#include <string>

class PresetManager
{
public:
	static constexpr uint8 PRESET_COUNT = 5;

private:
	struct Data
	{
	public:
		uint8 SelectedPresetIndex;
		Preset::Data Data[PRESET_COUNT];
	};

public:
	PresetManager(void)
		: m_PersistentData(0)
	{
	}

	void Initialize(void)
	{
		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			new (&m_Presets[i]) Preset();

		SetDataOnPresets();
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		m_Presets[m_PersistentData.Get().SelectedPresetIndex].Process(Buffer, Count);
	}

	Preset *GetSelectedPreset(void)
	{
		return &m_Presets[m_PersistentData.Get().SelectedPresetIndex];
	}

	uint8 GetSelectedPresetIndex(void)
	{
		return m_PersistentData.Get().SelectedPresetIndex;
	}

	void ChangeSelectedPreset(int8 Direction)
	{
		Data &data = m_PersistentData.Get();

		data.SelectedPresetIndex = Math::Wrap((int16)data.SelectedPresetIndex + Direction, 0, PRESET_COUNT - 1);

		m_Presets[data.SelectedPresetIndex].UpdateData();
	}

	void Save(void)
	{
		Data data;
		data.SelectedPresetIndex = m_PersistentData.Get().SelectedPresetIndex;

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			data.Data[i] = m_Presets[i].GetData();

		m_PersistentData.Set(data);

		PersistentBlobBase::SaveAll();
	}

	void Ereas(void)
	{
		PersistentBlobBase::EreasAll();

		SetDataOnPresets();
	}

private:
	void SetDataOnPresets(void)
	{
		static Data defaultData = {};
		defaultData.SelectedPresetIndex = 0;

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
		{
			Preset::Data &data = defaultData.Data[i];

			Preset::SetName(data, ("PRESET " + std::to_string(i + 1)).c_str());
			data.Volume = 1;

			uint8 effectIndex = 0;

			data.OverdriveData.Index = effectIndex++;
			data.OverdriveData.Enabled = true;
			data.OverdriveData.Rate = 1;
			data.OverdriveData.Gain = 0.5;

			data.ReverbData.Index = effectIndex++;
			data.ReverbData.Enabled = true;
			data.ReverbData.DelayTime = 0.5;
			data.ReverbData.Feedback = 0.6;
			data.ReverbData.WetRate = 0.5;
		}

		m_PersistentData.Initialize(defaultData);
		const Data &data = m_PersistentData.Get();

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			m_Presets[i].SetData(data.Data[i]);

		// Save();
	}

private:
	Preset m_Presets[PRESET_COUNT];

	PersistentBlob<Data> m_PersistentData;
};

#endif