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
	setNoteStealingEnabled(false);

	for (int i = 0; i < m_maxVoices; ++i)
		addVoice(new WaveTableVoice(m_tableGenerator.getTables(m_waveType)));

	addSound(new WaveTableSound);
}

void WaveSynthEngine::setAttack(float attack)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_attackTime = attack;
	}
}

void WaveSynthEngine::setDecay(float decay)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_decayTime = decay;
	}
}

void WaveSynthEngine::setSustain(float sustain)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_sustainLevel = sustain;
	}
}

void WaveSynthEngine::setRelease(float release)
{
	for (int i = 0; i < getNumVoices(); ++i)
	{
		auto* voice = dynamic_cast<WaveTableVoice*>(getVoice(i));
		voice->m_releaseTime = release;
	}
}

void WaveSynthEngine::setOscType(WaveType type)
{
	if (type != m_waveType)
	{
		m_waveType = type;
		allNotesOff(0, false);

		clearVoices();
		for (int i = 0; i < m_maxVoices; ++i)
			addVoice(new WaveTableVoice(m_tableGenerator.getTables(type)));
	}
}

//==============================================================================
