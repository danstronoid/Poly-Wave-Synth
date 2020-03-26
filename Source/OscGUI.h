/*
  ==============================================================================

    OscGUI.h
    Created: 18 Feb 2020 4:32:07pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomSlider.h"
#include "CustomBox.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================
/*
*/
class OscGUI    : public Component
{
public:
    OscGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        // Sliders
        oscType.setLookAndFeel(&customBox);
        oscType.addItem("Sine", 1);
        oscType.addItem("Square", 2);
        oscType.addItem("Saw", 3);
        oscType.addItem("Tri", 4);
        oscType.setJustificationType(Justification::centred);
        addAndMakeVisible(&oscType);
        oscTypeAttach.reset(new ComboBoxAttachment(parameters, "osc_oscType", oscType));

        levelSlider.setLookAndFeel(&customSlider);
        levelSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        levelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        levelSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&levelSlider);
        levelAttach.reset(new SliderAttachment(parameters, "osc_level", levelSlider));

        noiseSlider.setLookAndFeel(&customSlider);
        noiseSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        noiseSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        noiseSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&noiseSlider);
        noiseAttach.reset(new SliderAttachment(parameters, "osc_noise", noiseSlider));

        freqSlider.setLookAndFeel(&customSlider);
        freqSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        freqSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        freqSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&freqSlider);
        freqAttach.reset(new SliderAttachment(parameters, "osc_freq", freqSlider));

        // Labels
        oscLabel.setText("Osc", dontSendNotification);
        oscLabel.setFont(Font(20.0f, Font::bold));
        oscLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&oscLabel);

        levelLabel.setText("Level", dontSendNotification);
        levelLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&levelLabel);
        levelLabel.attachToComponent(&levelSlider, false);

        noiseLabel.setText("Noise", dontSendNotification);
        noiseLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&noiseLabel);
        noiseLabel.attachToComponent(&noiseSlider, false);

        freqLabel.setText("Freq", dontSendNotification);
        freqLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&freqLabel);
        freqLabel.attachToComponent(&freqSlider, false);
    }

    ~OscGUI()
    {
    }

    void paint (Graphics& /*g*/) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (Colours::white);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("OSC", getLocalBounds(), Justification::centred, true);
    }

    void resized() override
    {
        const int padding = 10;
        Rectangle<int> area = getLocalBounds().reduced(padding);
     
        int rotaryWidth = area.getWidth() / 3;
        int rotaryHeight = area.getHeight() / 3;
        int boxWidth = area.getWidth() / 3;
        int boxHeight = padding * 2;

        // top half
        area.removeFromTop(padding);
        oscLabel.setBounds(area.removeFromLeft(boxWidth).removeFromTop(rotaryHeight));
        Rectangle<int> boxArea = area.removeFromRight(boxWidth * 2).removeFromTop(rotaryHeight);
        oscType.setBounds(boxArea.getX(), boxArea.getY() + boxArea.getHeight() / 2 - boxHeight / 2,
            boxArea.getWidth(), boxHeight);

        // bottom half
        area = getLocalBounds().reduced(padding);
        area.removeFromBottom(padding);
        levelSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        noiseSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        freqSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
    }

private:
    AudioProcessorValueTreeState& parameters;

    CustomSlider customSlider;
    CustomBox customBox;

    ComboBox oscType;
    Slider levelSlider;
    Slider noiseSlider;
    Slider freqSlider;

    Label oscLabel;
    Label levelLabel;
    Label noiseLabel;
    Label freqLabel;

    std::unique_ptr<ComboBoxAttachment> oscTypeAttach;
    std::unique_ptr<SliderAttachment> levelAttach;
    std::unique_ptr<SliderAttachment> noiseAttach;
    std::unique_ptr<SliderAttachment> freqAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscGUI)
};

