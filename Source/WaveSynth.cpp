/*
  ==============================================================================

    Synth.cpp
    Created: 10 Feb 2020 10:53:16pm
    Author:  Daniel Schwartz

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

void WaveSynthEngine::setCurrentPlaybackSampleRate(double sampleRate)
{
	m_lfo.setSampleRate(sampleRate);

	Synthesiser::setCurrentPlaybackSampleRate(sampleRate);
}

void WaveSynthEngine::renderNextBlock(AudioBuffer<float>& output, const MidiBuffer& inputMidi, 
	int startSample, int numSamples)
{
	renderLfoBuffer(startSample, numSamples);
	Synthesiser::renderNextBlock(output, inputMidi, startSample, numSamples);
}

void WaveSynthEngine::renderLfoBuffer(int startSample, int numSamples)
{
	m_lfo.calculateDelta();

	// maybe move this to a prepare function
	m_lfoBuffer.setSize(1, numSamples);

	m_lfoBuffer.clear();

	while (--numSamples >= 0)
	{
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

void WaveSynthEngine::setFilterLFO(float rate, float depth)
{
	m_lfo.setParameters(rate, depth, 1, true, true);
}

//==============================================================================
