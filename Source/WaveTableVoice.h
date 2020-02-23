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
#include "Oscillator.h"

// these are implemented as structs for ease of changing parameters
struct WaveTableVoice : public SynthesiserVoice {
	WaveTableVoice(const std::vector<AudioSampleBuffer>& oscTables, const std::vector<AudioSampleBuffer>& modTables);

	bool isVoiceActive() const override;
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound,
		int /*currentPitchWheelPosition*/) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

	// not implemented
	void pitchWheelMoved(int /*newPitchWheelValue*/) override;
	void controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/) override;

	Random random;
	Oscillator m_osc;
	Oscillator m_fmOsc;
	StateVariableFilter m_svf;
	Envelope m_ampADSR;
	Envelope m_filterADSR;

	float m_envAmt{ 0.0f };
	float m_noise{ 0.0f };
};

//==============================================================================

struct WaveTableSound : public SynthesiserSound {
public:
	WaveTableSound();

	bool appliesToNote(int /*midiNoteNumber*/) override;
	bool appliesToChannel(int /*midiChannel*/) override;
};
