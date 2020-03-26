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
    ampEnvGUI(vts), oscGUI(vts), filterGUI(vts), filterEnvGUI(vts), fmGUI(vts), lfoGUI(vts)
{
    setSize (componentWidth * 3 + padding * 2, componentHeight * 2 + padding * 4);

    addAndMakeVisible(&ampEnvGUI);
    addAndMakeVisible(&oscGUI);
    addAndMakeVisible(&filterGUI);
    addAndMakeVisible(&filterEnvGUI);
    addAndMakeVisible(&fmGUI);
    addAndMakeVisible(&lfoGUI);
}

PolyWaveSynthAudioProcessorEditor::~PolyWaveSynthAudioProcessorEditor()
{
}

//==============================================================================
void PolyWaveSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::darkgrey.darker(1));

    // fill two separate sections for the osc and filter
    /*Rectangle<int> box(componentWidth * 3, componentHeight);
    g.setColour(Colours::darkgrey);
    box.setPosition(padding, padding * 2);
    g.fillRect(box.toFloat());

    g.setColour(Colours::darkslategrey);
    box.setPosition(padding, componentHeight + padding * 2);
    g.fillRect(box.toFloat());*/
}

void PolyWaveSynthAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(padding);
    area.removeFromTop(padding);
    oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    ampEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    fmGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

    area = getLocalBounds().reduced(padding);
    area.removeFromTop(componentHeight + padding);
    filterGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    lfoGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
