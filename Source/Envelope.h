/*
  ==============================================================================

    Envelope.h
    Created: 13 Feb 2020 12:49:10pm
    Author:  Daniel Schwartz

	This is a modified version of the JUCE ADSR class.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Envelope {
public:
	Envelope();

	void setSampleRate(double sampleRate);
	void setParameters(float attack, float decay, float sus, float release);
	float getNextSample();
	void updateRates();

	void noteOn();
	void noteOff();

	bool isActive() const;
	void reset();

private:
	enum class State { idle, attack, decay, sustain, release };
	State currentState = State::idle;

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