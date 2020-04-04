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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyWaveSynthAudioProcessorEditor::PolyWaveSynthAudioProcessorEditor (PolyWaveSynthAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), parameters (vts), 
    ampEnvGUI(vts), oscGUI(vts), filterGUI(vts), filterEnvGUI(vts), fmGUI(vts), lfoGUI(vts)
{
    setSize (componentWidth * 3 + padding * 2, componentHeight * 2 + padding * 3);
    Timer::startTimerHz(30);

    // title
    title.setText("Tiny Table", dontSendNotification);
    title.setFont(Font(40.0f, Font::bold));
    title.setJustificationType(Justification::centredTop);
    addAndMakeVisible(&title);

    // master output control
    outSlider.setLookAndFeel(&customSlider);
    outSlider.setSliderStyle(Slider::LinearHorizontal);
    outSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 60, 20);
    addAndMakeVisible(&outSlider);
    outAttach.reset(new SliderAttachment(parameters, "gain", outSlider));

    outLabel.setLookAndFeel(&customLabel);
    outLabel.setText("Output", dontSendNotification);
    outLabel.attachToComponent(&outSlider, true);
    addAndMakeVisible(&outLabel);

    // gui componenets
    addAndMakeVisible(&meter);

    addAndMakeVisible(&ampEnvGUI);
    addAndMakeVisible(&oscGUI);
    addAndMakeVisible(&filterGUI);
    addAndMakeVisible(&filterEnvGUI);
    addAndMakeVisible(&fmGUI);
    addAndMakeVisible(&lfoGUI);
}

PolyWaveSynthAudioProcessorEditor::~PolyWaveSynthAudioProcessorEditor()
{
    Timer::stopTimer();
}

void PolyWaveSynthAudioProcessorEditor::timerCallback()
{
    meter.update(processor.getMeterValue());
    meter.repaint();
}

//==============================================================================
void PolyWaveSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (palette.window);

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

    // title area
    title.setBounds(area.removeFromLeft(componentWidth).removeFromTop(padding)); 
    outSlider.setBounds(area.removeFromRight(componentWidth).removeFromTop(padding));

    // meter area
    area = getLocalBounds();
    area.reduce(padding, padding / 2);
    meter.setBounds(area.removeFromRight(componentWidth).removeFromTop(padding / 2));

    // top row
    area = getLocalBounds().reduced(padding);
    area.removeFromTop(padding);
    oscGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    ampEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    fmGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

    // bottom row
    area = getLocalBounds().reduced(padding);
    area.removeFromTop(componentHeight + padding);
    filterGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    filterEnvGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
    lfoGUI.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
}
