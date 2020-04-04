/*
  ==============================================================================

    StateVariableFilter.h
    Created: 19 Feb 2020 9:52:28am
    Author:  Daniel Schwartz

    This is an implementation of the algorithm described by the technical paper
    https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf by Andrew Simper

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

enum class State { Low_Pass, High_Pass, Band_Pass };

class StateVariableFilter
{
public:
    StateVariableFilter() {}

    // the sample rate must be set before processing
    void setSampleRate(double sampleRate)
    {
        m_sampleRate = sampleRate;
        reset();
    }

    void setParameters(State state, double cutoff, double q)
    {
        m_currentState = state;
        m_cutoff = cutoff;
        m_targetCutoff = cutoff;
        m_q = q;
        calculateCoef();
    }

    // this update function can be call with an envelope value or an osc mod value 
    // modulating the cutoff with the provided values
    void update(double env = 1, double mod = 0)
    {
        m_cutoff = fmin(19000, fmax(40, m_targetCutoff * env + mod * m_targetCutoff / 2));
        calculateCoef();
    }

    // renders the filter on a single sample and returns the filtered sample value
    inline float renderSample(float v0)
    {
        m_v3 = v0 - m_ic2eq;
        m_v1 = m_a1 * m_ic1eq + m_a2 * m_v3;
        m_v2 = m_ic2eq + m_a2 * m_ic1eq + m_a3 * m_v3;
        m_ic1eq = 2 * m_v1 - m_ic1eq;
        m_ic2eq = 2 * m_v2 - m_ic2eq;

        return static_cast<float>(m_m0 * v0 + m_m1 * m_v1 + m_m2 * m_v2);
    }

    // optional function to render samples as a block instead of returning a single sample
    inline void renderBlock(float* samples, const int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            samples[i] = renderSample(samples[i]);
    }

    void reset()
    {
        m_ic1eq = 0;
        m_ic2eq = 0;
    }

private:
    State m_currentState{ State::Low_Pass };
    double m_sampleRate{};
    double m_cutoff{};
    double m_q{};

    // the target cutoff is the central cutoff of the filter
    // the m_cutoff can be modulated around this center
    double m_targetCutoff{};

    double m_ic1eq{ 0 };
    double m_ic2eq{ 0 };
    double m_v1{ 0 };
    double m_v2{ 0 };
    double m_v3{ 0 };

    //coefficients 
    double m_a1{ 0 };
    double m_a2{ 0 };
    double m_a3{ 0 };
    double m_m0{ 0 };
    double m_m1{ 0 };
    double m_m2{ 0 };

    // this private member function calculates all of the coefficients to be used 
    // in render sample, this is called when a parameter is changed
    void calculateCoef()
    {
        double g = std::tan(MathConstants<double>::pi * m_cutoff / m_sampleRate);
        double k = 1 / m_q;

        switch (m_currentState)
        {
        case State::Low_Pass:
            m_a1 = 1 / (1 + g * (g + k));
            m_a2 = g * m_a1;
            m_a3 = g * m_a2;
            m_m0 = 0;
            m_m1 = 0;
            m_m2 = 1;
            break;

        case State::Band_Pass:
            m_a1 = 1 / (1 + g * (g + k));
            m_a2 = g * m_a1;
            m_a3 = g * m_a2;
            m_m0 = 0;
            m_m1 = 1;
            m_m2 = 0;
            break;

        case State::High_Pass:
            m_a1 = 1 / (1 + g * (g + k));
            m_a2 = g * m_a1;
            m_a3 = g * m_a2;
            m_m0 = 1;
            m_m1 = -k;
            m_m2 = -1;
            break;

        }
    }
};