/*
  ==============================================================================

    OscGUI.h
    Created: 18 Feb 2020 4:32:07pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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
        oscType.addItem("Sine", 1);
        oscType.addItem("Square", 2);
        oscType.addItem("Saw", 3);
        oscType.addItem("Tri", 4);
        oscType.setJustificationType(Justification::centred);
        addAndMakeVisible(&oscType);
        oscTypeAttach.reset(new ComboBoxAttachment(parameters, "oscType", oscType));

        gainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        gainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        gainSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&gainSlider);
        gainAttach.reset(new SliderAttachment(parameters, "gain", gainSlider));

        noiseSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        noiseSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        noiseSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&noiseSlider);
        noiseAttach.reset(new SliderAttachment(parameters, "noise", noiseSlider));

        fmSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        fmSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        fmSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&fmSlider);

        // Labels
        oscLabel.setText("Osc", dontSendNotification);
        oscLabel.setFont(Font(20.0f, Font::bold));
        oscLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&oscLabel);

        gainLabel.setText("Gain", dontSendNotification);
        gainLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&gainLabel);
        gainLabel.attachToComponent(&gainSlider, false);

        noiseLabel.setText("Noise", dontSendNotification);
        noiseLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&noiseLabel);
        noiseLabel.attachToComponent(&noiseSlider, false);

        fmLabel.setText("FM", dontSendNotification);
        fmLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&fmLabel);
        fmLabel.attachToComponent(&fmSlider, false);
    }

    ~OscGUI()
    {
    }

    void paint (Graphics& g) override
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
        gainSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        noiseSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        fmSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
    }

private:
    AudioProcessorValueTreeState& parameters;

    ComboBox oscType;
    Slider gainSlider;
    Slider noiseSlider;
    Slider fmSlider;

    Label oscLabel;
    Label gainLabel;
    Label noiseLabel;
    Label fmLabel;

    std::unique_ptr<ComboBoxAttachment> oscTypeAttach;
    std::unique_ptr<SliderAttachment> gainAttach;
    std::unique_ptr<SliderAttachment> noiseAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscGUI)
};
