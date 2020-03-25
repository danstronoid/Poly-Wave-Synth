/*
  ==============================================================================

    WaveTableVoice.cpp
    Created: 15 Feb 2020 3:58:31pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "WaveTableVoice.h"

WaveTableVoice::WaveTableVoice(const WaveTableGenerator& tableGenerator,
	AudioBuffer<float>& controlBuffer) :
	m_osc(tableGenerator), m_fmOsc(tableGenerator), m_lfoBuffer(controlBuffer)
{
	// initialize the sampleRate for all of the components
	m_osc.setSampleRate(getSampleRate());
	m_fmOsc.setSampleRate(getSampleRate());
	m_svf.setSampleRate(getSampleRate());
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

void WaveTableVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* /*sound*/,
	int /*currentPitchWheelPosition*/)
{	
	m_osc.noteOn(midiNoteNumber, velocity);
	m_fmOsc.noteOn(midiNoteNumber, velocity);
	m_ampADSR.noteOn();
	m_filterADSR.noteOn();
}

void WaveTableVoice::stopNote(float /*velocity*/, bool allowTailOff)
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
		// get a read pointer from the control buffer
		auto* lfoValues = m_lfoBuffer.getReadPointer(0);

		while (--numSamples >= 0)
		{
			// if the ampEnv is not active, we can clear the note and reset
			if (!m_ampADSR.isActive())
			{
				m_ampADSR.reset();
				m_filterADSR.reset();
				clearCurrentNote();
				m_osc.reset();
			}
			// calculate and apply FM to the osc delta
			float fmValue = m_fmOsc.getNextSample();
			m_osc.calculateDelta(fmValue);

			// get the current sample from the osc and get a noise value
			float currentSample = m_osc.getNextSample();
			float noise = random.nextFloat() * m_noise - (m_noise / 2);

			// get the envelope and control values
			float ampEnvValue = m_ampADSR.getNextSample();
			float filterEnvValue = m_filterADSR.getNextSample() * m_envAmt + (1 - m_envAmt);
			float lfoValue = lfoValues[startSample];

			// update the filter with the envelope and modulation and apply it to the sample
			m_svf.update(filterEnvValue, lfoValue);
			currentSample = m_svf.renderSample(currentSample + noise);

			// apply the ampEnv to the sample and add it to the output buffer
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

