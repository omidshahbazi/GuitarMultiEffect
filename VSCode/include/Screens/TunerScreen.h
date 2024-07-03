#pragma once
#ifndef TUNER_SCREEN_H
#define TUNER_SCREEN_H

#include "Screen.h"
#include "../framework/DSP/FrequencyDetector.h"
#include "../framework/DSP/Notes.h"

class TunerScreen : public Screen
{
private:
	struct StringNameFrequency
	{
	public:
		cstr Name;
		float Frequency;
	};

	static constexpr uint16 SAMPLE_COUNT = 1024;
	static constexpr float MAX_THRESHOLD = 3;

public:
	TunerScreen(PresetManager *PresetManager, ControlManager *ControlManager)
		: Screen(PresetManager, ControlManager),
		  m_RecordedSamples(0)
	{
	}

	void Update(void) override
	{
		if (m_RecordedSamples < SAMPLE_COUNT)
			return;

		m_Frequency = m_FreqDetector.CalculateFrequency();

		m_RecordedSamples = 0;

		MarkAsDirty();
	}

	void ProcessAudioBuffer(const SampleType *const InputBuffer, const SampleType *const OutputBuffer, uint8 Count) override
	{
		if (m_RecordedSamples >= SAMPLE_COUNT)
			return;

		for (uint8 i = 0; i < Count; ++i)
			m_FreqDetector.Record(InputBuffer[i]);

		m_RecordedSamples += Count;
	}

protected:
	void Draw(LCDCanvas &Canvas) override
	{
		Canvas.Clear(COLOR_GRAY);

		DrawHeader(Canvas, DEFAULT_HEADER_HEIGHT,
				   HEADER_DEFAULT_LEFT_BOX_COLOR, nullptr, {}, {},
				   HEADER_DEFAULT_MIDDLE_BOX_COLOR, "TUNER", HEADER_DEFAULT_MIDDLE_TEXT_FONT, HEADER_DEFAULT_MIDDLE_TEXT_COLOR,
				   HEADER_DEFAULT_RIGHT_BOX_COLOR, nullptr, {}, {});

		StringNameFrequency *nameFreq = FindClosest(m_Frequency);
		if (nameFreq == nullptr)
			return;

		const Point CENTER_POSITION = {Canvas.GetDimension().X * 0.5, 100};
		const uint8 CIRCLE_RADIUS = 15;
		Rect rect = {{CENTER_POSITION.X - CIRCLE_RADIUS, (uint16)(100 - CIRCLE_RADIUS)}, {CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2}};

		Canvas.DrawFilledCircle(CENTER_POSITION, CIRCLE_RADIUS + 5, COLOR_LIGHT_BLUE);

		float freqDiff = m_Frequency - nameFreq->Frequency;

		const uint16 MAX_FLOATING_DISTANCE = 50;
		Point indicaotrPosition = CENTER_POSITION;
		indicaotrPosition.X += Math::Clamp(freqDiff / MAX_THRESHOLD, -1, 1) * MAX_FLOATING_DISTANCE;

		Color indicatorColor = COLOR_RED;
		if (Math::Absolute(freqDiff) < 1)
			indicatorColor = COLOR_GREEN;

		Canvas.DrawFilledCircle(indicaotrPosition, CIRCLE_RADIUS, indicatorColor);

		DrawStringJustified(Canvas, rect, nameFreq->Name, FONT_32, COLOR_BLACK);
	}

	static StringNameFrequency *FindClosest(float Frequency)
	{
		StringNameFrequency *nameFreqs = GetNameFrequencies();
		uint8 closesIndex = 0;
		float closestDistance = MAX_FREQUENCY;
		for (uint8 i = 0; i < 6; ++i)
		{
			float distance = Math::Absolute(Frequency - nameFreqs[i].Frequency);
			if (distance >= closestDistance)
				continue;

			closesIndex = i;
			closestDistance = distance;
		}

		if (Frequency < nameFreqs[0].Frequency || nameFreqs[5].Frequency < Frequency)
			if (closestDistance > 100)
				return nullptr;

		return &nameFreqs[closesIndex];
	}

	static StringNameFrequency *GetNameFrequencies(void)
	{
		static StringNameFrequency nameFrequencies[6];
		static bool isInitialized = false;
		if (!isInitialized)
		{
			nameFrequencies[0] = {"E", NOTE_E2};
			nameFrequencies[1] = {"A", NOTE_A2};
			nameFrequencies[2] = {"D", NOTE_D3};
			nameFrequencies[3] = {"G", NOTE_G3};
			nameFrequencies[4] = {"B", NOTE_B3};
			nameFrequencies[5] = {"E", NOTE_E4};

			isInitialized = true;
		}

		return nameFrequencies;
	}

private:
	FrequencyDetector<SampleType, SAMPLE_RATE, SAMPLE_COUNT> m_FreqDetector;
	uint16 m_RecordedSamples;
	float m_Frequency;
};

#endif