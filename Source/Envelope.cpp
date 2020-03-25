/*
  ==============================================================================

    Envelope.cpp
    Created: 17 Feb 2020 6:52:32pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#include "Envelope.h"

Envelope::Envelope() {}

void Envelope::setSampleRate(double sampleRate)
{
	m_sampleRate = sampleRate;
}

void Envelope::setParameters(float attack, float decay, float sus, float release)
{
	m_susTarget = sus;
	m_attackTime = attack;
	m_decayTime = decay;
	m_releaseTime = release;
}

float Envelope::getNextSample()
{
	switch (currentState)
	{
	case (State::idle):
		return 0.0f;

	case (State::attack):
		m_envValue += m_attackRate; //* ((1.0f / 0.63f) - m_envValue);

		if (m_envValue >= 1.0f)
		{
			m_envValue = 1.0f;

			currentState = State::decay;
		}
		break;

	case (State::decay):
		m_envValue -= m_decayRate;

		if (m_envValue <= m_susLevel)
		{
			m_envValue = m_susLevel;
			currentState = State::sustain;
		}
		break;

	case (State::sustain):
		m_envValue = m_susLevel;
		break;

	case (State::release):
		m_envValue -= m_releaseRate;

		if (m_envValue <= 0.0f)
		{
			reset();
		}
		break;
	default:
		return 0.0f;
	}

	return m_envValue;
}

void Envelope::updateRates()
{
	m_susLevel = m_susTarget;

	m_attackRate = static_cast<float>(1.0f / (m_attackTime * m_sampleRate));
	m_decayRate = static_cast<float>((1.0f - m_susLevel) / (m_decayTime * m_sampleRate));
	m_releaseRate = static_cast<float>(m_susLevel / (m_releaseTime * m_sampleRate));

	reset();
}

void Envelope::noteOn()
{
	updateRates();
	currentState = State::attack;
	m_envValue = 0;
}

void Envelope::noteOff()
{
	if (currentState != State::sustain)
	{
		m_releaseRate = static_cast<float> (m_envValue / (m_releaseTime * m_sampleRate));
		m_fastRelease = true;
	}
	currentState = State::release;
}

bool Envelope::isActive() const
{
	return currentState != State::idle;
}

void Envelope::reset()
{
	m_envValue = 0;
	currentState = State::idle;

	if (m_fastRelease)
	{
		m_releaseRate = static_cast<float> (m_susLevel / (m_releaseTime * m_sampleRate));
	}
}