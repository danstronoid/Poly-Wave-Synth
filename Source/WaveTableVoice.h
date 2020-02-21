/*
  ==============================================================================

    WaveTableVoice.h
    Created: 15 Feb 2020 3:58:31pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
#include "StateVariableFilter.h"

class WaveTableVoice : public SynthesiserVoice {
public:
	WaveTableVoice(const std::vector<AudioSampleBuffer>& tables);

	bool isVoiceActive() const override;
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound,
		int /*currentPitchWheelPosition*/) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

	// not implemented
	void pitchWheelMoved(int /*newPitchWheelValue*/) override;
	void controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/) override;

	// these are public for ease of changing parameters
	StateVariableFilter m_svf;
	Envelope m_ampADSR;
	Envelope m_filterADSR;
	float m_envAmt{ 0.0f };
	float m_level{ 0.0f };
	float m_noise{ 0.0f };

private:
	// maintain a reference to the tableGenerator owned by the WaveSynthEngine
	//const AudioSampleBuffer& m_table;

	const std::vector<AudioSampleBuffer>& m_tables;
	int m_octave{};

	int m_tableSize{};
	float m_tablePos{ 0.0f };
	float m_tableDelta{ 0.0f };

	Random random;
};

//==============================================================================

class WaveTableSound : public SynthesiserSound {
public:
	WaveTableSound();

	bool appliesToNote(int /*midiNoteNumber*/) override;
	bool appliesToChannel(int /*midiChannel*/) override;
};
