/*
  ==============================================================================

    Envelope.h
    Created: 13 Feb 2020 12:49:10pm
    Author:  Daniel Schwartz

	This is a linear ADSR envelope class based on the JUCE ADSR.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Envelope {
public:
	Envelope();

	// the sample rate must be set before processing the envelope
	void setSampleRate(double sampleRate);

	// set attack time, decay time, sustain level, and release time
	void setParameters(float attack, float decay, float sus, float release);

	// processes one sample of the envelope, updates the envValue and the currentState
	float getNextSample();

	// calculates rates based on the times provided by setParameters
	// this is called in noteOn
	void updateRates();

	// trigger the attack state with noteOn and the release state with noteOff
	void noteOn();
	void noteOff(bool allowTailOff = true);

	// checks if the envelope is in its idle state
	bool isActive() const;

	// resets the envelope state and value
	void reset();

private:
	// this state machine determines the behaviour of the envelope
	enum class State { idle, attack, decay, sustain, release };
	State currentState = State::idle;

	// this is a very fast release time to ease a hard stop
	const float m_fastReleaseTime{ 0.01f };

	double m_sampleRate{ 0 };
	float m_envValue{ 0.0f };
	bool m_fastRelease{ false };

	float m_attackRate{ 0.0f };
	float m_decayRate{ 0.0f };
	float m_releaseRate{ 0.0f };
	float m_susLevel{ 0.0f };

	float m_attackTime{ 0.0f };
	float m_decayTime{ 0.0f };
	float m_releaseTime{ 0.0f };
	float m_susTarget{ 0.0f };
};