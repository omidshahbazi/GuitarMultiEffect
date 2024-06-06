#pragma once
#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "Preset.h"
#include "../framework/DSP/PersistentBlob.h"
#include <string>

class PresetManager
{
public:
	static constexpr uint8 PRESET_COUNT = 2;

private:
	struct Data
	{
	public:
		Data(void)
			: Size(sizeof(Data)),
			  SelectedPresetIndex(0)
		{
		}

		uint8 SelectedPresetIndex;
		Preset::Data PresetData[PRESET_COUNT];
		uint16 Size;
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

		UpdatePresetData();
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

		UpdatePresetData();
	}

	void Save(void)
	{
		Data data;
		data.SelectedPresetIndex = m_PersistentData.Get().SelectedPresetIndex;

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			data.PresetData[i] = m_Presets[i].GetData();

		m_PersistentData.Set(data);

		PersistentBlobBase::SaveAll();
	}

	void Ereas(void)
	{
		PersistentBlobBase::EreasAll();

		SetDataOnPresets();
	}

private:
	void UpdatePresetData(void)
	{
		m_Presets[m_PersistentData.Get().SelectedPresetIndex].UpdateData();
	}

	void SetDataOnPresets(void)
	{
		static Data defaultData = {};

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
		{
			Preset::Data &data = defaultData.PresetData[i];

			uint8 effectIndex = 0;

			data.FXData.Index = effectIndex++;
			data.DsData.Index = effectIndex++;
			data.EqData.Index = effectIndex++;
			data.RevData.Index = effectIndex++;
			data.ModData.Index = effectIndex++;
		}

		m_PersistentData.Initialize(defaultData);
		const Data &data = m_PersistentData.Get();

		if (data.Size != defaultData.Size)
			m_PersistentData.Set(defaultData);

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			m_Presets[i].SetData(data.PresetData[i]);
	}

private:
	Preset m_Presets[PRESET_COUNT];

	PersistentBlob<Data> m_PersistentData;
};

#endif