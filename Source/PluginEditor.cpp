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
    setSize (componentWidth * 3, componentHeight * 2);

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

    Rectangle<float> box(componentWidth * 2.0f, componentHeight);
    g.setColour(Colours::darkgrey);
    g.fillRoundedRectangle(box, 10);

    g.setColour(Colours::darkslategrey);
    box.setPosition(0, componentHeight);
    g.fillRoundedRectangle(box, 10);
}

void PolyWaveSynthAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    ampEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

    area = getLocalBounds();
    filterGUI.setBounds(area.removeFromLeft(componentWidth).removeFromBottom(componentHeight));
    filterEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromBottom(componentHeight));
}
