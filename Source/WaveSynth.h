/*
  ==============================================================================

    Synth.h
    Created: 10 Feb 2020 10:53:16pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveTableGenerator.h"
#include "WaveTableVoice.h"


class WaveSynthEngine : public Synthesiser {
public:
	WaveSynthEngine();

	void setOscType(WaveType type);
	void setFMOscType(WaveType type);

	void setOscParameters(float noise, float freq);
	void setModParameters(float multi, float depth, float freq);
	void setAmpADSR(float attack, float decay, float sustain, float release);

	void setFilterParameters(State state, double cutoff, double q, float envAmt);
	void setFilterADSR(float attack, float decay, float sustain, float release);

private:
	WaveTableGenerator m_tableGenerator;
	const int m_maxVoices{ 8 };
	WaveType m_waveType{ WaveType::SINE };
	WaveType m_fmWaveType{ WaveType::SINE };
};

//==============================================================================

