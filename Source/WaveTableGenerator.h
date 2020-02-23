/*
  ==============================================================================

    WaveTableGenerator.h
    Created: 11 Feb 2020 6:19:54pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum class WaveType
{
	SINE = 0,
	SQUARE = 1,
	SAW = 2,
	TRI = 3
};

class WaveTableGenerator
{
public:
	WaveTableGenerator();

	const std::vector<AudioSampleBuffer>& getTables(WaveType type) const;
	const AudioSampleBuffer& getTable(WaveType type) const;
	const int getTableSize() const;

private:
	// methods to generate the different wave types, called in constructor
	void generateSineTable();
	void generateSquareTable();
	void generateSawTable();
	void generateTriTable();

	// all generation methods call these two functions
	void fourierTable(AudioSampleBuffer& buffer, int harm, float phase, const Array<float> &amps);
	void normalizeTable(AudioSampleBuffer& buffer);

private:
	const int m_tableSize{ 2048 };
	const int m_octDivision{ 10 };

	std::vector<AudioSampleBuffer> m_sineTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_squareTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_sawTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_triTable{ m_octDivision };
};