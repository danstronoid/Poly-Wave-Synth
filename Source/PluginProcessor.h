/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

    Tiny Table
    Author:  Daniel Schwartz

    Tiny Table is a simple wave table synthesizer.  Each voice has 2 oscillators, 
    2 ADSR envelopes, and one filter.  There is also a single LFO which controls 
    the cutoff frequency of all of the filters.  There are a total of 12 voices.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveSynth.h"

//==============================================================================
/**
*/
class PolyWaveSynthAudioProcessor  : public AudioProcessor,
									 public AudioProcessorValueTreeState::Listener

{
public:
    //==============================================================================
    PolyWaveSynthAudioProcessor();
    ~PolyWaveSynthAudioProcessor();

    //==============================================================================

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    /* These are helper functions to update parameters */

    // create the parameter layout for the constructor
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // add all parameter listeners
    void addParameterListeners();

    // executes callback when a parameter is changed
    void parameterChanged(const String& parameterID, float newValue);

    // callback functions to update the different components
    void updateOsc();
    void updateFM();
    void updateAmpEnv();
    void updateFilter();
    void updateFilterEnv();
    void updateFilterLFO();

    // initialze all parameters
    void initParameters();

    // returns the current output meter value when called
    float getMeterValue() { return meter; };

private:
	AudioProcessorValueTreeState parameters;
	WaveSynthEngine synthEngine;

	float currentGain{ 1.0f };
	float previousGain{ 1.0f };

    // keep track of the magnitude of the output buffer to send to output meter
    float meter{ 0.0f };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyWaveSynthAudioProcessor)
};
