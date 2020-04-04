/*
  ==============================================================================

    LfoGUI.h
    Created: 23 Feb 2020 1:56:21pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomLookAndFeel.h"
#include "CustomSliderTypes.h"

typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/*
*/
class LfoGUI    : public Component
{
public:
    LfoGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        // Box
        lfoType.setLookAndFeel(&customBox);
        lfoType.addItem("Sine", 1);
        lfoType.addItem("Square", 2);
        lfoType.addItem("Sawtooth", 3);
        lfoType.addItem("Triangle", 4);
        lfoType.setJustificationType(Justification::centred);
        addAndMakeVisible(&lfoType);
        typeAttach.reset(new ComboBoxAttachment(parameters, "lfo_type", lfoType));

        // Sliders
        rateSlider.setLookAndFeel(&customSlider);
        rateSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        rateSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        rateSlider.setTextValueSuffix(" Hz");
        //rateSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&rateSlider);
        rateAttach.reset(new SliderAttachment(parameters, "lfo_rate", rateSlider));
        
        depthSlider.setLookAndFeel(&customSlider);
        depthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        depthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        //depthSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&depthSlider);
        depthAttach.reset(new SliderAttachment(parameters, "lfo_depth", depthSlider));

        // Buttons
        triggerButton.setLookAndFeel(&customButton);
        triggerButton.setButtonText("Trigger");
        addAndMakeVisible(&triggerButton);
        triggerAttach.reset(new ButtonAttachment(parameters, "lfo_trigger", triggerButton));

        // Labels
        lfoLabel.setLookAndFeel(&customLabel);
        lfoLabel.setText("LFO", dontSendNotification);
        lfoLabel.setFont(Font(20.0f, Font::bold));
        lfoLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&lfoLabel);

        rateLabel.setLookAndFeel(&customLabel);
        rateLabel.setText("Rate", dontSendNotification);
        rateLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&rateLabel);
        rateLabel.attachToComponent(&rateSlider, false);

        depthLabel.setLookAndFeel(&customLabel);
        depthLabel.setText("Depth", dontSendNotification);
        depthLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&depthLabel);
        depthLabel.attachToComponent(&depthSlider, false);

    }

    ~LfoGUI()
    {
    }

    void paint (Graphics& /*g*/) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (Colours::grey);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }

    void resized() override
    {
        const int padding = 10;
        Rectangle<int> area = getLocalBounds().reduced(padding);

        int rotaryWidth = area.getWidth() / 3;
        int rotaryHeight = area.getHeight() / 2;
        int boxWidth = area.getWidth() / 3;
        int boxHeight = padding * 2;

        // top half
        area.removeFromTop(padding);
        lfoLabel.setBounds(area.removeFromLeft(boxWidth).removeFromTop(rotaryHeight / 2));
        Rectangle<int> boxArea = area.removeFromRight(boxWidth * 2).removeFromTop(rotaryHeight / 2);
        lfoType.setBounds(boxArea.getX(), boxArea.getY() + boxArea.getHeight() / 2 - boxHeight / 2,
            boxArea.getWidth(), boxHeight);

        // bottom half
        area = getLocalBounds().reduced(padding);
        area.removeFromBottom(padding);
        rateSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        depthSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        triggerButton.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
    }

private:
    AudioProcessorValueTreeState& parameters;

    CustomSlider customSlider;
    CustomBox customBox;
    CustomLabel customLabel;
    CustomButton customButton;

    ComboBox lfoType;

    Slider rateSlider;
    PercentSlider depthSlider;

    ToggleButton triggerButton;

    Label rateLabel;
    Label depthLabel; 
    Label lfoLabel;

    std::unique_ptr<SliderAttachment> rateAttach;
    std::unique_ptr<SliderAttachment> depthAttach;
    std::unique_ptr<ButtonAttachment> triggerAttach;
    std::unique_ptr<ComboBoxAttachment> typeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoGUI)
};
