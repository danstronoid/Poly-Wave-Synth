/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AmpEnvGUI.h"
#include "OscGUI.h"
#include "FilterGUI.h"
#include "FilterEnvGUI.h"
#include "FmGUI.h"
#include "LfoGUI.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class PolyWaveSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    PolyWaveSynthAudioProcessorEditor (PolyWaveSynthAudioProcessor&, AudioProcessorValueTreeState&);
    ~PolyWaveSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PolyWaveSynthAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;

    AmpEnvGUI ampEnvGUI;
    OscGUI oscGUI;
    FilterGUI filterGUI;
    FilterEnvGUI filterEnvGUI;
    FmGUI fmGUI;
    LfoGUI lfoGUI;

    const int componentWidth = 200;
    const int componentHeight = 200;
    const int padding = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyWaveSynthAudioProcessorEditor)
};
