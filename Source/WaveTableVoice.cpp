/*
  ==============================================================================

    WaveTableVoice.cpp
    Created: 15 Feb 2020 3:58:31pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "WaveTableVoice.h"

WaveTableVoice::WaveTableVoice(const std::vector<AudioSampleBuffer>& oscTables, const std::vector<AudioSampleBuffer>& modTables) :
	m_osc(oscTables), m_fmOsc(modTables)
{
	m_osc.setSampleRate(getSampleRate());
	m_fmOsc.setSampleRate(getSampleRate());
	m_fmOsc.setParameters(0, 1);

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
	m_osc.noteOn(midiNoteNumber, velocity);
	m_fmOsc.noteOn(midiNoteNumber, velocity);
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
		m_osc.reset();
	}
}

void WaveTableVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (m_osc.isActive())
	{

		while (--numSamples >= 0)
		{
			if (!m_ampADSR.isActive())
			{
				m_ampADSR.reset();
				m_filterADSR.reset();
				clearCurrentNote();
				m_osc.reset();
			}
			float fmValue = m_fmOsc.getNextSample();

			m_osc.calculateDelta(fmValue);

			float currentSample = m_osc.getNextSample();

			float ampEnvValue = m_ampADSR.getNextSample();
			float filterEnvValue = m_filterADSR.getNextSample() * m_envAmt + (1 - m_envAmt);

			float noise = random.nextFloat() * m_noise - (m_noise / 2);

			m_svf.update(filterEnvValue);
			currentSample = m_svf.renderSample(currentSample + noise);

			for (auto channel = outputBuffer.getNumChannels(); --channel >= 0;)
				outputBuffer.addSample(channel, startSample, currentSample * ampEnvValue);

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

