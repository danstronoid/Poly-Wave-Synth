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

	void setAttack(float attack);
	void setDecay(float decay);
	void setSustain(float sustain);
	void setRelease(float release);
	void setOscType(WaveType type);

private:
	WaveTableGenerator m_tableGenerator;
	const int m_maxVoices{ 12 };
	WaveType m_waveType{ SINE };
};

//==============================================================================

