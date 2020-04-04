/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

    Tiny Table
    Author:  Daniel Schwartz

    Tiny Table is a simple wave table synthesizer.  Each voice has 2 oscillators,
    2 ADSR envelopes, and one filter.  There is also a single LFO which controls
    the cutoff frequency of all of the filters.  There are a total of 12 voices.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "CustomSliderTypes.h"
#include "LevelMeter.h"

// all of the GUI components to place in the editor window
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
class PolyWaveSynthAudioProcessorEditor  : public AudioProcessorEditor,
                                           public Timer
{
public:
    PolyWaveSynthAudioProcessorEditor (PolyWaveSynthAudioProcessor&, AudioProcessorValueTreeState&);
    ~PolyWaveSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PolyWaveSynthAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;

    ColorPalette palette;

    CustomSlider customSlider;
    CustomLabel customLabel;

    LevelMeter meter;
    DecibelSlider outSlider;

    Label outLabel;
    Label title;

    std::unique_ptr<SliderAttachment> outAttach;

    AmpEnvGUI ampEnvGUI;
    OscGUI oscGUI;
    FilterGUI filterGUI;
    FilterEnvGUI filterEnvGUI;
    FmGUI fmGUI;
    LfoGUI lfoGUI;

    const int componentWidth = 240;
    const int componentHeight = 240;
    const int padding = 40;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolyWaveSynthAudioProcessorEditor)
};
