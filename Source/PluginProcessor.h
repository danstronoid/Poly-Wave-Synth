/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveSynth.h"
#include "Filter.h"

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
	void parameterChanged(const String& parameterID, float newValue);

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

private:
	AudioProcessorValueTreeState parameters;
	WaveSynthEngine synthEngine;
    std::vector<StateVariableFilter> svf{ 2 };

	float currentGain{ 0.6f };
	float previousGain{ 0.6f };

	const StringArray oscTypes = { "Sine", "Square", "Saw", "Tri" };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyWaveSynthAudioProcessor)
};
