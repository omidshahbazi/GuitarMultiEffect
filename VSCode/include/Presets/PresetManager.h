#pragma once
#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

#include "Preset.h"
#include "Rhythm.h"
#include "../framework/DSP/IHAL.h"
#include "../framework/DSP/PersistentBlob.h"
#include "../framework/DSP/DSPs/Looper.h"

class PresetManager
{
private:
	// static constexpr uint16 MAX_LOOPER_TIME = 2 * 60;
	static constexpr uint16 MAX_LOOPER_TIME = 10;

public:
	static constexpr uint8 PRESET_COUNT = 4;

	typedef Looper<SampleType, SAMPLE_RATE, MAX_LOOPER_TIME> LooperType;

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
		  m_Looper(nullptr),
		  m_Rhythm(nullptr)
	{
	}

	void Initialize(void)
	{
		PersistentBlobBase::EreasAll();

		m_Looper = Memory::Allocate<LooperType>();
		new (m_Looper) LooperType();

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

		m_Looper->ProcessBuffer(Buffer, Count);

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

	IHAL *GetHAL(void) const
	{
		return m_HAL;
	}

	Rhythm *GetRhythm(void)
	{
		return m_Rhythm;
	}

	LooperType *GetLooper(void)
	{
		return m_Looper;
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
			data.InputGain = 30;
			data.OutputGain = -20;
			data.OutputVolume = 0.2;

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
			data.InputGain = 40;
			data.OutputGain = -20;
			data.OutputVolume = 0.2;

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
			data.InputGain = 10;
			data.OutputGain = -10;

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
	LooperType *m_Looper;
	Rhythm *m_Rhythm;
};

#endif