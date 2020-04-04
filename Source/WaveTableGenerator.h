/*
  ==============================================================================

    WaveTableGenerator.h
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

	// this returns a vector of bandlimited tables for a given wavetype
	const std::vector<AudioSampleBuffer>& getTables(WaveType type) const;

	// this returns a single table 
	// the table it returns is table[0] which contains the full amount of harmonic detail
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
	// this table generator uses a constant tablesize 
	// the table size could be decreased as the octave is increased for better performance
	// however, using a constant table size decreases the likelyhood of indexing outside the bounds of the table
	const int m_tableSize{ 2048 };

	// this table generator uses 10 octave divisions for each wavetype
	// it might be better to use more divisions maybe along perfect 5ths instead of octaves?
	const int m_octDivision{ 10 };

	std::vector<AudioSampleBuffer> m_sineTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_squareTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_sawTable{ m_octDivision };
	std::vector<AudioSampleBuffer> m_triTable{ m_octDivision };
};