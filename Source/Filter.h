/*
  ==============================================================================

    Filter.h
    Created: 17 Feb 2020 10:04:30am
    Author:  Daniel Schwartz

    This is an implementation for a stereo set of filters.  Need to reimplement
    as one filter per voice.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
#include "StateVariableFilter.h"

//
//class Filter
//{
//public:
//    Filter(int numOutputChannels) : m_svf(numOutputChannels)
//    {
//
//    }
//
//    void prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
//    {
//        m_adsr.setSampleRate(sampleRate);
//        m_adsr.set(0.1f, 0.1f, 1.0f, 0.3f);
//
//        for (int channel = 0; channel < m_svf.size(); ++channel)
//        {
//            m_svf[channel].setSampleRate(sampleRate);
//            m_svf[channel].setParameters(State::Low_Pass, 4000, 1);
//        }
//    }
//
//    void releaseResources()
//    {
//
//    }
//
//    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
//    {
//        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
//        {
//            MidiBuffer::Iterator iterator(midiMessages);
//            MidiMessage message;
//            int sampleNumber;
//
//            auto* samples = buffer.getWritePointer(channel);
//
//            for (int i = 0; i < buffer.getNumSamples(); ++i)
//            {
//                iterator.getNextEvent(message, sampleNumber);
//
//                if (message.isNoteOn() && !m_adsr.isActive())
//                    m_adsr.noteOn();
//                else if (message.isNoteOff())
//                    m_adsr.noteOff();
//
//                m_svf[channel].update(4000.0 * m_adsr.getNextSample());
//
//                samples[i] = m_svf[channel].renderSample(samples[i]);
//            }
//        }
//    }
//
//    void setFilterParameters(State state, double cutoff, double q)
//    {
//        for (int channel = 0; channel < m_svf.size(); ++channel)
//            m_svf[channel].setParameters(state, cutoff, q);
//    }
//
//private:
//    // need to find the right type of thread-safe array for filters
//    std::vector<StateVariableFilter> m_svf;
//    Envelope m_adsr;
//};