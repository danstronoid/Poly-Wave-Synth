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
#include "Oscillator.h"


class WaveSynthEngine : public Synthesiser {
public:
	WaveSynthEngine();

	void setCurrentPlaybackSampleRate(double sampleRate) override;
	void renderNextBlock(AudioBuffer<float>& output, const MidiBuffer& inputMidi, 
		int startSample, int numSamples);
	void renderLfoBuffer(int startSample, int numSamples);


	void setOscType(WaveType type);
	void setFMOscType(WaveType type);

	void setOscParameters(float level, float noise, float freq, bool fixed);
	void setFMParameters(float multi, float depth, float freq, bool fixed);
	void setAmpADSR(float attack, float decay, float sustain, float release);

	void setFilterParameters(State state, double cutoff, double q, float envAmt);
	void setFilterADSR(float attack, float decay, float sustain, float release);
	void setFilterLFOType(WaveType type);
	void setFilterLFO(float rate, float depth);

private:
	const int m_maxVoices{ 8 };

	// this table generator belongs to the synth so that it can be passed around
	// without making a bunch of copies
	WaveTableGenerator m_tableGenerator;

	// the synth engine has an lfo that's written to a buffer and fed to the voices
	Oscillator m_lfo;
	AudioBuffer<float> m_lfoBuffer;
};

//==============================================================================

