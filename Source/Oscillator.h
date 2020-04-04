/*
  ==============================================================================

    Oscillator.h
    Created: 22 Feb 2020 1:38:05pm
    Author:  Daniel Schwartz

    This osciallator class uses wavetables created by the WaveTableGenerator class.
    The WaveTableGenerator is passed by reference so that each oscillator doesn't need
    it's own copy.

    This osc can be used with noteOn() for midi input, or as a fixed frequency oscillator 
    with setParameters().

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveTableGenerator.h"

class Oscillator
{
public:
    // the oscillator class needs a reference to the tables created by
    // WaveTableGenerator
    Oscillator(const WaveTableGenerator& tableGenerator) :
        m_tables(tableGenerator)
    {
        m_tableSize = m_tables.getTableSize();
    }

    // the sample rate must be set before processing
    void setSampleRate(double sampleRate)
    {
        m_sampleRate = sampleRate;
        m_sizeOverSR = static_cast<float>(m_tableSize / m_sampleRate);
    }

    void setOscType(WaveType type)
    {
        m_waveType = type;
    }

    // this method is used to set the oscillator using a fixed frequency
    // if fixedFreq is false, the provided frequency will be ignored
    // the overall level of the osc can be set using oscLevel
    // use the initLvl bool to initialize output of a fixed osc
    void setParameters(float freq, float oscLevel = 0.3, float multi = 1, 
        bool fixedFreq = false, bool initLvl = false)
    {
        m_multi = multi;
        m_oscLevel = oscLevel;
        m_fixedFreq = fixedFreq;

        if (m_fixedFreq)
            m_freq = freq;

        // this bool only needs to be used if the level must be initialized
        if (initLvl)
            m_level = m_oscLevel;    
    }

    // calculates the delta given a modulation index, use this with a fixed osc
    void calculateDelta(float mod = 0)
    {
        m_tableDelta = fmax(0.0f, m_freq + mod * m_freq) * m_sizeOverSR;
    }

    // set the osc using a midi note on message
    void noteOn(int midiNoteNumber, float velocity)
    {
        m_tablePos = 0;
        m_level = velocity * m_oscLevel;

        // calculates the octave from the midi note, this is used for a different bandlimited table per octave
        m_octave = midiNoteNumber / 12 - 1;

        // if this is a fixed osc, then we ignore the midi note number
        if (!m_fixedFreq)
            m_freq = static_cast<float>(MidiMessage::getMidiNoteInHertz(midiNoteNumber));

        m_tableDelta = (m_freq * m_multi) * m_sizeOverSR;
    }

    // this calculates the next sample using simple linear interpolation
    forcedinline float getNextSample() noexcept
    {
        int index0 = static_cast<int>(m_tablePos);
        int index1 = index0 + 1;

        float frac = m_tablePos - static_cast<float>(index0);

        const float* table = m_tables.getTables(m_waveType)[m_octave].getReadPointer(0);
        float value0 = table[index0];
        float value1 = table[index1];

        float currentSample = value0 + frac * (value1 - value0);

        if ((m_tablePos += m_tableDelta) > m_tableSize)
            m_tablePos -= m_tableSize;

        // might want to ramp level for depth change
        return currentSample * m_level;
    }

    // reset the table position only
    void resetPos()
    {
        m_tablePos = 0;
    }

    // reset table position and delta
    void reset()
    {
        m_tablePos = 0;
        m_tableDelta = 0;
    }

    // if delta is 0, the osc isn't active
    bool isActive()
    {
        return m_tableDelta != 0;
    }

private:
    // each oscillator has a reference to all of the tables in the table generator
    // for ease of switching osc types
    const WaveTableGenerator& m_tables;
    WaveType m_waveType{ WaveType::SINE };
    int m_octave{};

    // all parameters needed to calculate delta
    double m_sampleRate{};
    float m_sizeOverSR{};
    int m_tableSize{};
    float m_tablePos{};
    float m_tableDelta{};

    // the max level for the osc, 0.3f is a modest level
    float m_oscLevel{ 0.3f };
    // the output level (m_oscLevel * velocity)
    float m_level{};

    // the frequency of the osc
    float m_freq{};
    // this tests whether the osc uses a fixed frequency or response to note on
    bool m_fixedFreq{};
    // this is a frequency multiplier for FM 
    float m_multi{ 1.0f };
};