#pragma once
#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "Preset.h"
#include "Rhythm.h"
#include "../framework/DSP/IHAL.h"
#include "../framework/DSP/PersistentBlob.h"

class PresetManager
{
public:
	static constexpr uint8 PRESET_COUNT = 4;

private:
	struct Data
	{
	public:
		Data(void)
			: SelectedPresetIndex(0),
			  PresetData{},
			  Size(sizeof(Data))
		{
		}

		uint8 SelectedPresetIndex;
		Preset::Data PresetData[PRESET_COUNT];
		Rhythm::Data RhythmData;
		uint16 Size;
	};

public:
	PresetManager(IHAL *HAL)
		: m_HAL(HAL),
		  m_PersistentData(0),
		  m_Rhythm(nullptr)
	{
	}

	void Initialize(void)
	{
		PersistentBlobBase::EreasAll();

		m_Rhythm = Memory::Allocate<Rhythm>();
		new (m_Rhythm) Rhythm(m_HAL);

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			new (&m_Presets[i]) Preset();

		SetDataOnPresets();

		UpdatePresetData();
	}

	void Update(void)
	{
		m_Rhythm->Update();
	}

	void Process(SampleType *Buffer, uint8 Count)
	{
		auto &data = m_PersistentData.Get();

		m_Presets[data.SelectedPresetIndex].Process(Buffer, Count);

		m_Rhythm->Process(Buffer, Count);
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

	Rhythm *GetRhythm(void)
	{
		return m_Rhythm;
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

		uint8 presetIndex = 0;

		// CLEAN
		{
			Preset::Data &data = defaultData.PresetData[presetIndex++];
			Preset::SetName(data, "CLEAN");

#ifdef ADD_REV_EFFECT
			data.RevData.Enabled = true;
#endif
		}

		// OVERDRIVE
		{
			Preset::Data &data = defaultData.PresetData[presetIndex++];
			Preset::SetName(data, "OVERDRIVE");

#ifdef ADD_DS_EFFECT
			data.DsData.Enabled = true;
			data.DsData.Type = DsEffect::Data::Types::Overdrive;
#endif

#ifdef ADD_REV_EFFECT
			data.RevData.Enabled = true;
#endif
		}

		// DISTORTION
		{
			Preset::Data &data = defaultData.PresetData[presetIndex++];
			Preset::SetName(data, "DISTORTION");

#ifdef ADD_DS_EFFECT
			data.DsData.Enabled = true;
			data.DsData.Type = DsEffect::Data::Types::Distortion;
#endif

#ifdef ADD_REV_EFFECT
			data.RevData.Enabled = true;
#endif
		}

		// FUZZ
		{
			Preset::Data &data = defaultData.PresetData[presetIndex++];
			Preset::SetName(data, "FUZZ");

#ifdef ADD_DS_EFFECT
			data.DsData.Enabled = true;
			data.DsData.Type = DsEffect::Data::Types::Fuzz;
#endif

#ifdef ADD_REV_EFFECT
			data.RevData.Enabled = true;
#endif
		}

		m_PersistentData.Initialize(defaultData);
		const Data &data = m_PersistentData.Get();

		if (data.Size != defaultData.Size)
			m_PersistentData.Set(defaultData);

		for (uint8 i = 0; i < PRESET_COUNT; ++i)
			m_Presets[i].SetData(data.PresetData[i]);

		m_Rhythm->SetData(data.RhythmData);
	}

private:
	IHAL *m_HAL;
	Preset m_Presets[PRESET_COUNT];
	PersistentBlob<Data> m_PersistentData;
	Rhythm *m_Rhythm;
};

#endif