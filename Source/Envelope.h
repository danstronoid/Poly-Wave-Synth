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
	Envelope() {}

	void setSampleRate(int sampleRate)
	{
		m_sampleRate = sampleRate;
	}

	void set(float attack, float decay, float sus, float release)
	{
		m_susLevel = sus;

		m_attackTime = attack;
		m_decayTime = decay;
		m_releaseTime = release;

		m_attackRate = static_cast<float>(1.0f / (m_attackTime * m_sampleRate));
		m_decayRate = static_cast<float>((1.0f - m_susLevel) / (m_decayTime * m_sampleRate));
		m_releaseRate = static_cast<float>(m_susLevel / (m_releaseTime * m_sampleRate));
		
		reset();
	}

	float getNextSample()
	{
		switch (currentState)
		{
		case (State::idle) :
			return 0.0f;

		case (State::attack) :
			m_envValue += m_attackRate; //* ((1.0f / 0.63f) - m_envValue);

			if (m_envValue >= 1.0f)
			{
				m_envValue = 1.0f;

				currentState = State::decay;
			}
			break;

		case (State::decay) :
			m_envValue -= m_decayRate;

			if (m_envValue <= m_susLevel)
			{
				m_envValue = m_susLevel;
				currentState = State::sustain;
			}
			break;

		case (State::sustain) :
			m_envValue = m_susLevel;
			break;

		case (State::release) :
			m_envValue -= m_releaseRate;

			if (m_envValue <= 0.0f)
			{
				reset();
			}
			break;
		default :
			return 0.0f;
		}

		return m_envValue;
	}
	
	void noteOn()
	{
		currentState = State::attack;
		m_envValue = 0;
	}

	void noteOff()
	{
		if (currentState != State::sustain)
		{
			m_releaseRate = static_cast<float> (m_envValue / (m_releaseTime * m_sampleRate));
			m_fastRelease = true;
		}
		currentState = State::release;
	}

	bool isActive() const
	{
		return currentState != State::idle;
	}

	void reset()
	{
		m_envValue = 0;
		currentState = State::idle;
		
		if (m_fastRelease)
		{
			m_releaseRate = static_cast<float> (m_susLevel / (m_releaseTime * m_sampleRate));
		}
	}

private:
	enum class State { idle, attack, decay, sustain, release };
	State currentState = State::idle;

	int m_sampleRate{ 0 };
	float m_envValue{ 0.0f };
	bool m_fastRelease{ false };

	float m_attackRate{ 0.0f };
	float m_decayRate{ 0.0f };
	float m_releaseRate{ 0.0f };
	float m_susLevel{ 0.0f };

	float m_attackTime{ 0.0f };
	float m_decayTime{ 0.0f };
	float m_releaseTime{ 0.0f };
};