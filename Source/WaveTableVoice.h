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
#include "WaveTableGenerator.h"

//==============================================================================

// these are implemented as structs for ease of changing parameters
struct WaveTableVoice : public SynthesiserVoice {
	WaveTableVoice(const WaveTableGenerator& tableGenerator, AudioBuffer<float>& controlBuffer);

	bool isVoiceActive() const override;
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound,
		int /*currentPitchWheelPosition*/) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

	// not implemented
	void pitchWheelMoved(int /*newPitchWheelValue*/) override;
	void controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/) override;

	//==============================================================================

	// noise generator
	Random random;
	float m_noise{ 0.0f };

	// two oscillators
	Oscillator m_osc;
	Oscillator m_fmOsc;

	// filter
	StateVariableFilter m_svf;

	// amp and filter envelopes
	Envelope m_ampADSR;
	Envelope m_filterADSR;
	float m_envAmt{ 0.0f };

	// a control buffer
	AudioBuffer<float>& m_lfoBuffer;
};

//==============================================================================

struct WaveTableSound : public SynthesiserSound {
public:
	WaveTableSound();

	bool appliesToNote(int /*midiNoteNumber*/) override;
	bool appliesToChannel(int /*midiChannel*/) override;
};
