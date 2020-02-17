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

	float m_attackTime{ 0.05f };
	float m_decayTime{ 0.01f };
	float m_sustainLevel{ 1.0f };
	float m_releaseTime{ 0.1f };

private:
	// maintain a reference to the tableGenerator owned by the WaveSynthEngine
	//const AudioSampleBuffer& m_table;

	const std::vector<AudioSampleBuffer>& m_tables;
	int m_octave{};

	int m_tableSize{};
	float m_tablePos{ 0.0f };
	float m_tableDelta{ 0.0f };

	float m_level{ 0.0f };

	Envelope m_adsr;
};

//==============================================================================

class WaveTableSound : public SynthesiserSound {
public:
	WaveTableSound();

	bool appliesToNote(int /*midiNoteNumber*/) override;
	bool appliesToChannel(int /*midiChannel*/) override;
};
