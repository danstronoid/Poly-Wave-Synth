/*
  ==============================================================================

    Synth.cpp
    Created: 10 Feb 2020 10:53:16pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "WaveSynth.h"

WaveSynthEngine::WaveSynthEngine()
{
	setNoteStealingEnabled(true);

	for (int i = 0; i < m_maxVoices; ++i)
		addVoice(new WaveTableVoice(m_tableGenerator.getTables(m_waveType), 
			m_tableGenerator.getTables(m_fmWaveType)));

	addSound(new WaveTableSound);
}

void WaveSynthEngine::setOscType(WaveType type)
{
	if (type != m_waveType)
	{
		m_waveType = type;
		allNotesOff(0, false);

		clearVoices();
		for (int i = 0; i < m_maxVoices; ++i)
			addVoice(new WaveTableVoice(m_tableGenerator.getTables(type), 
				m_tableGenerator.getTables(m_fmWaveType)));
	}
}

void WaveSynthEngine::setFMOscType(WaveType type)
{
	if (type != m_fmWaveType)
	{
		m_fmWaveType = type;
		allNotesOff(0, false);

		clearVoices();
		for (int i = 0; i < m_maxVoices; ++i)
			addVoice(new WaveTableVoice(m_tableGenerator.getTables(m_waveType), 
				m_tableGenerator.getTables(type)));
	}
}

void WaveSynthEngine::setOscParameters(float noise, float freq)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_noise = noise;
		voice->m_osc.setParameters(1, 0.3, freq);
	}
}

void WaveSynthEngine::setModParameters(float multi, float depth, float freq)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_fmOsc.setParameters(multi, depth, freq);
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

//==============================================================================
