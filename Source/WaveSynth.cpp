/*
  ==============================================================================

    Synth.cpp
    Created: 10 Feb 2020 10:53:16pm
    Author:  Daniel Schwartz

	This is the synth engine itself that inherits from the JUCE synthesizer.

  ==============================================================================
*/

#include "WaveSynth.h"

WaveSynthEngine::WaveSynthEngine() :
	m_lfo(m_tableGenerator)
{
	setNoteStealingEnabled(true);

	for (int i = 0; i < m_maxVoices; ++i)
		addVoice(new WaveTableVoice(m_tableGenerator, m_lfoBuffer));

	addSound(new WaveTableSound);
}

// prepareToPlay must be called before processing
void WaveSynthEngine::prepareToPlay(double currentSampleRate, int samplesPerBlock)
{
	// allocates space for the lfoBuffer
	m_lfo.setSampleRate(currentSampleRate);
	m_lfoBuffer.setSize(1, samplesPerBlock);

	Synthesiser::setCurrentPlaybackSampleRate(currentSampleRate);
}

// this is an override but not marked as such because the complier doesn't recognize it as one
// I think this is because renderNextBlock is actually a template function?
// this implementation is used to call renderLfoBuffer before rendering the voices
void WaveSynthEngine::renderNextBlock(AudioBuffer<float>& output, const MidiBuffer& inputMidi, 
	int startSample, int numSamples)
{
	renderLfoBuffer(inputMidi, startSample, numSamples);
	Synthesiser::renderNextBlock(output, inputMidi, startSample, numSamples);
}

// renders the lfo signal to a buffer so that all voices render the same lfo signal
void WaveSynthEngine::renderLfoBuffer(const MidiBuffer& inputMidi, int startSample, int numSamples)
{
	m_lfo.calculateDelta();

	m_lfoBuffer.clear();

	MidiBuffer::Iterator midiIt(inputMidi);
	MidiMessage m;
	int midiPos;

	while (--numSamples >= 0)
	{
		midiIt.setNextSamplePosition(startSample);

		// if the lfo is set to retrigger, reset the osc position on a note on message
		if (m_lfoReTrigger && midiIt.getNextEvent(m, midiPos) && startSample == midiPos && m.isNoteOn())
		{
			m_lfo.resetPos();
		}

		float currentSample = m_lfo.getNextSample();
		m_lfoBuffer.addSample(0, startSample, currentSample);
		++startSample;
	}
}

//==============================================================================
// Set functions

void WaveSynthEngine::setOscType(WaveType type)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_osc.setOscType(type);
	}
}

void WaveSynthEngine::setFMOscType(WaveType type)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_fmOsc.setOscType(type);
	}
}

void WaveSynthEngine::setOscParameters(float level, float noise, float freq, bool fixed)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_noise = noise;
		voice->m_osc.setParameters(freq, level, 1, fixed);
	}
}

void WaveSynthEngine::setFMParameters(float freq, float depth, float multi, bool fixed)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_fmOsc.setParameters(freq, depth, multi, fixed);
	}
}

void WaveSynthEngine::setAmpADSR(float attack, float decay, float sustain, float release)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_ampADSR.setParameters(attack, decay, sustain, release);
	}
}

void WaveSynthEngine::setFilterParameters(State state, double cutoff, double q, float envAmt)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_svf.setParameters(state, cutoff, q);
		voice->m_envAmt = envAmt;
	}
}

void WaveSynthEngine::setFilterADSR(float attack, float decay, float sustain, float release)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_filterADSR.setParameters(attack, decay, sustain, release);
	}
}

void WaveSynthEngine::setFilterLFOType(WaveType type)
{
	m_lfo.setOscType(type);
}

void WaveSynthEngine::setFilterLFO(float rate, float depth, bool trigger)
{
	m_lfo.setParameters(rate, depth, 1, true, true);
	m_lfoReTrigger = trigger;
}

//==============================================================================
