/*
  ==============================================================================

    WaveTableVoice.cpp
    Created: 15 Feb 2020 3:58:31pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "WaveTableVoice.h"

WaveTableVoice::WaveTableVoice(const std::vector<AudioSampleBuffer>& tables) :
	m_tables(tables)
{
	m_svf.setSampleRate(getSampleRate());
	m_svf.setParameters(State::Low_Pass, 400, 1);

	m_ampADSR.setSampleRate(getSampleRate());
	m_filterADSR.setSampleRate(getSampleRate());
}

bool WaveTableVoice::isVoiceActive() const
{
	if ((getCurrentlyPlayingNote() >= 0))
		return true;
	else
		return false;
}

bool WaveTableVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<WaveTableSound*>(sound) != nullptr;
}

void WaveTableVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound,
	int /*currentPitchWheelPosition*/)
{
	m_tablePos = 0;
	m_level = velocity * 0.3f;

	m_octave = midiNoteNumber / 12 - 1;
	m_tableSize = m_tables[m_octave].getNumSamples() - 1;

	float frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	float sizeOverSR = (float)m_tableSize / getSampleRate();
	m_tableDelta = frequency * sizeOverSR;

	m_ampADSR.noteOn();
	m_filterADSR.noteOn();
}

void WaveTableVoice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff)
	{
		m_ampADSR.noteOff();
		m_filterADSR.noteOff();
	}
	else
	{
		m_ampADSR.reset();
		m_filterADSR.reset();
		clearCurrentNote();
		m_tableDelta = 0;
	}
}

void WaveTableVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (m_tableDelta != 0)
	{

		while (--numSamples >= 0)
		{
			float ampEnvValue = m_ampADSR.getNextSample();
			float filterEnvValue = m_filterADSR.getNextSample() * m_envAmt + (1 - m_envAmt);

			if (!m_ampADSR.isActive())
			{
				m_ampADSR.reset();
				m_filterADSR.reset();
				clearCurrentNote();
				m_tableDelta = 0;
			}

			int index0 = (int)m_tablePos;
			int index1 = index0 + 1;

			float frac = m_tablePos - (float)index0;

			const float* table = m_tables[m_octave].getReadPointer(0);
			float value0 = table[index0];
			float value1 = table[index1];

			float currentSample = value0 + frac * (value1 - value0);

			float noise = random.nextFloat() * m_noise - (m_noise / 2);

			m_svf.update(filterEnvValue);
			currentSample = m_svf.renderSample(currentSample * m_level + noise);

			for (auto channel = outputBuffer.getNumChannels(); --channel >= 0;)
				outputBuffer.addSample(channel, startSample, currentSample * ampEnvValue);

			if ((m_tablePos += m_tableDelta) > m_tableSize)
				m_tablePos -= m_tableSize;

			++startSample;
		}
	}
}

void WaveTableVoice::pitchWheelMoved(int /*newPitchWheelValue*/) {}
void WaveTableVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/) {}
//==============================================================================

WaveTableSound::WaveTableSound()
{
}

bool WaveTableSound::appliesToNote(int /*midiNoteNumber*/)
{
	return true;
}

bool WaveTableSound::appliesToChannel(int /*midiChannel*/)
{
	return true;
}

