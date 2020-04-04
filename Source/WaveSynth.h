/*
  ==============================================================================

    Synth.h
    Created: 10 Feb 2020 10:53:16pm
    Author:  Daniel Schwartz

	This is the synth engine itself that inherits from the JUCE synthesizer.

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

	// prepareToPlay must be called before processing
	void prepareToPlay(double currentSampleRate, int samplesPerBlock);

	// this is an override but not marked as such because the complier doesn't recognize it as one
	// I think this is because renderNextBlock is actually a template function?
	// this implementation is used to call renderLfoBuffer before rendering the voices
	void renderNextBlock(AudioBuffer<float>& output, const MidiBuffer& inputMidi, 
		int startSample, int numSamples);

	// renders the lfo signal to a buffer so that all voices render the same lfo signal
	void renderLfoBuffer(const MidiBuffer& inputMidi, int startSample, int numSamples);

	// set functions for all of the parameters to be controlled by the pluginEditor
	void setOscType(WaveType type);
	void setFMOscType(WaveType type);

	void setOscParameters(float level, float noise, float freq, bool fixed);
	void setFMParameters(float multi, float depth, float freq, bool fixed);
	void setAmpADSR(float attack, float decay, float sustain, float release);

	void setFilterParameters(State state, double cutoff, double q, float envAmt);
	void setFilterADSR(float attack, float decay, float sustain, float release);
	void setFilterLFOType(WaveType type);
	void setFilterLFO(float rate, float depth, bool trigger);

private:
	// this is the maximum number of voices to allocate
	// 12 should provide an ample number of voices without causing problems
	const int m_maxVoices{ 12 };

	// this table generator belongs to the synth so that it can be passed around
	// without making a bunch of copies
	WaveTableGenerator m_tableGenerator;

	// the synth engine has an lfo that's written to a buffer and fed to the voices
	Oscillator m_lfo;
	bool m_lfoReTrigger{false};
	AudioBuffer<float> m_lfoBuffer;
};

//==============================================================================

