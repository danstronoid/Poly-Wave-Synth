/*
  ==============================================================================

    WaveTableGenerator.cpp
    Created: 11 Feb 2020 6:19:54pm
    Author:  Daniel Schwartz

	This WaveTableGenerator generates and allocates memory for 4 different kinds
	of wavetables.  A single instance of a WaveTableGenerator can be created and
	passed around to avoid making unnecessary copies of the wave table.

	This creates sine, square, saw, and tri tables.  It would be cool to add
	more functionality to this class to create custom tables using the fourierTable
	function.

	This table generator uses a constant tablesize.  The table size could be decreased
	as the octave is increased for better performance.  However, using a constant table
	size decreases the likelyhood of indexing outside the bounds of the table.

	The tables are bandlimited with one table per octave, but it might be better
	to use more divisions at some point.

	The number of harmonics per octave is calcualated by:

		tableSize / 2^(octave + 1) - 1

  ==============================================================================
*/

#include "WaveTableGenerator.h"

WaveTableGenerator::WaveTableGenerator()
{
	generateSineTable();
	generateSquareTable();
	generateSawTable();
	generateTriTable();
}

// this returns a vector of bandlimited tables for a given wavetype
const std::vector<AudioSampleBuffer>& WaveTableGenerator::getTables(WaveType type) const
{
	switch (type)
	{
	case WaveType::SINE:
		return m_sineTable;
	case WaveType::SQUARE:
		return m_squareTable;
	case WaveType::SAW:
		return m_sawTable;
	case WaveType::TRI:
		return m_triTable;
	default:
		return m_sineTable;
	}
}

// this returns a single table 
// the table it returns is table[0] which contains the full amount of harmonic detail
const AudioSampleBuffer& WaveTableGenerator::getTable(WaveType type) const
{
	switch (type)
	{
	case WaveType::SINE:
		return m_sineTable[0];
	case WaveType::SQUARE:
		return m_squareTable[0];
	case WaveType::SAW:
		return m_sawTable[0];
	case WaveType::TRI:
		return m_triTable[0];
	default:
		return m_sineTable[0];
	}
}

const int WaveTableGenerator::getTableSize() const
{
	return m_tableSize;
}

//==============================================================================

// Private member functions

void WaveTableGenerator::generateSineTable()
{
	for (int octave = 0; octave < m_octDivision; ++octave)
	{
		m_sineTable[octave].setSize(1, m_tableSize + 1);
		m_sineTable[octave].clear();

		Array<float> amps;

		fourierTable(m_sineTable[octave], 1, 0.25, amps);
	}
}

void WaveTableGenerator::generateSquareTable()
{
	for (int octave = 0; octave < m_octDivision; ++octave)
	{
		m_squareTable[octave].setSize(1, m_tableSize + 1);
		m_squareTable[octave].clear();

		int harm = static_cast<int>(m_tableSize / std::pow(2, octave + 1) - 1);
		Array<float> amps;

		for (int i = 0; i < harm; i += 2)
			amps.insert(i, 1.0f / (i + 1.0f));

		fourierTable(m_squareTable[octave], harm, 0.25, amps);

		normalizeTable(m_squareTable[octave]);
	}
}

void WaveTableGenerator::generateSawTable()
{
	for (int octave = 0; octave < m_octDivision; ++octave)
	{
		m_sawTable[octave].setSize(1, m_tableSize + 1);
		m_sawTable[octave].clear();

		int harm = static_cast<int>(m_tableSize / std::pow(2, octave + 1) - 1);
		Array<float> amps;

		for (int i = 0; i < harm; ++i)
			amps.insert(i, 1.0f / (i + 1.0f));

		fourierTable(m_sawTable[octave], harm, 0.25, amps);

		normalizeTable(m_sawTable[octave]);
	}
}

void WaveTableGenerator::generateTriTable()
{
	for (int octave = 0; octave < m_octDivision; ++octave)
	{
		m_triTable[octave].setSize(1, m_tableSize + 1);
		m_triTable[octave].clear();

		int harm = static_cast<int>(m_tableSize / std::pow(2, octave + 1) - 1);
		Array<float> amps;

		for (int i = 0; i < harm; i += 2)
			amps.insert(i, 1.0f / ((i + 1.0f) * (i + 1.0f)));

		fourierTable(m_triTable[octave], harm, 0, amps);

		normalizeTable(m_triTable[octave]);
	}
}

void WaveTableGenerator::fourierTable(AudioSampleBuffer& buffer, int harm, float phase, const Array<float>& amps)
{
	float* samples = buffer.getWritePointer(0);
	phase *= MathConstants<float>::twoPi;

	for (int i = 0; i < harm; ++i)
	{
		double currentAngle = phase;
		double angleDelta = (i + 1) * (MathConstants<double>::twoPi / m_tableSize);
		float amp = amps.isEmpty() ? 1.0f : amps[i];

		for (int n = 0; n < m_tableSize; ++n)
		{
			double sample = std::cos(currentAngle);
			samples[n] += (float)(sample * amp);
			currentAngle += angleDelta;
		}
	}

	samples[m_tableSize] = samples[0];
}

void WaveTableGenerator::normalizeTable(AudioSampleBuffer& buffer)
{
	float maxAmp{ 0.0f };
	float amp;

	float* samples = buffer.getWritePointer(0);

	for (int i = 0; i < m_tableSize; ++i)
	{
		amp = samples[i];
		if (amp > maxAmp)
			maxAmp = amp;
	}

	maxAmp = 1.0f / maxAmp;

	for (int i = 0; i < m_tableSize; ++i)
		samples[i] *= maxAmp;

	samples[m_tableSize] = samples[0];
}