/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyWaveSynthAudioProcessorEditor::PolyWaveSynthAudioProcessorEditor (PolyWaveSynthAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), parameters (vts), 
    ampEnvGUI(vts), oscGUI(vts), filterGUI(vts), filterEnvGUI(vts)
{
    setSize (400, 400);

    addAndMakeVisible(&ampEnvGUI);
    addAndMakeVisible(&oscGUI);
    addAndMakeVisible(&filterGUI);
    addAndMakeVisible(&filterEnvGUI);
}

PolyWaveSynthAudioProcessorEditor::~PolyWaveSynthAudioProcessorEditor()
{
}

//==============================================================================
void PolyWaveSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
}

void PolyWaveSynthAudioProcessorEditor::resized()
{
    const int componentWidth = 200;
    const int componentHeight = 200;

    Rectangle<int> area = getLocalBounds();
    oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    ampEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

    area = getLocalBounds();
    filterGUI.setBounds(area.removeFromLeft(componentWidth).removeFromBottom(componentHeight));
    filterEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromBottom(componentHeight));
}
