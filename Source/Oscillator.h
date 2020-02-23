/*
  ==============================================================================

    Oscillator.h
    Created: 22 Feb 2020 1:38:05pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// this can be used with noteOn() for midi input, 
// or as a static oscillator with setParameters() 
class Oscillator
{
public:
    Oscillator(const std::vector<AudioSampleBuffer>& tables) :
        m_tables(tables)
    {
        m_tableSize = m_tables[m_octave].getNumSamples() - 1;
    }

    void setSampleRate(int sampleRate)
    {
        m_sampleRate = sampleRate;
        m_sizeOverSR = static_cast<float>(m_tableSize) / m_sampleRate;
    }

    void setParameters(float multi = 1, float depth = 0.3, float fixedFreq = 0)
    {
        m_multi = multi;
        m_oscLevel = depth;
        m_fixedFreq = fixedFreq;

        if (m_fixedFreq > 40)
            m_freq = m_fixedFreq;
    }

    void calculateDelta(float mod = 0)
    {
        m_tableDelta = fmax(0, m_freq + mod * m_freq) * m_sizeOverSR;
    }


    void noteOn(int midiNoteNumber, float velocity)
    {
        m_tablePos = 0;
        m_level = velocity * m_oscLevel;

        // need to alter this so that the octave is calculated from the frequency 
        m_octave = midiNoteNumber / 12 - 1;

        if (m_fixedFreq > 40)
            m_freq = m_fixedFreq;
        else
            m_freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

        m_tableDelta = (m_freq * m_multi) * m_sizeOverSR;
    }

    forcedinline float getNextSample() noexcept
    {
        int index0 = static_cast<int>(m_tablePos);
        int index1 = index0 + 1;

        float frac = m_tablePos - static_cast<float>(index0);

        const float* table = m_tables[m_octave].getReadPointer(0);
        float value0 = table[index0];
        float value1 = table[index1];

        float currentSample = value0 + frac * (value1 - value0);

        if ((m_tablePos += m_tableDelta) > m_tableSize)
            m_tablePos -= m_tableSize;

        // might want to ramp level for depth change
        return currentSample * m_level;
    }

    void resetPos()
    {
        m_tablePos = 0;
    }

    void reset()
    {
        m_tablePos = 0;
        m_tableDelta = 0;
    }

    bool isActive()
    {
        return m_tableDelta != 0;
    }

private:
    const std::vector<AudioSampleBuffer>& m_tables;
    int m_octave{};

    int m_sampleRate{};
    float m_level{};
    float m_oscLevel{ 0.3f };

    float m_multi{ 1.0f };
    float m_freq{};
    float m_fixedFreq{};

    int m_tableSize{};
    float m_sizeOverSR{};
    float m_tablePos{};
    float m_tableDelta{};
};