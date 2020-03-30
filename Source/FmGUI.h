/*
  ==============================================================================

    ModGUI.h
    Created: 22 Feb 2020 9:33:44am
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomSlider.h"
#include "CustomBox.h"
#include "PercentSlider.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================
/*
*/
class FmGUI    : public Component
{
public:
    FmGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        // Box
        oscType.setLookAndFeel(&customBox);
        oscType.addItem("Sine", 1);
        oscType.addItem("Square", 2);
        oscType.addItem("Sawtooth", 3);
        oscType.addItem("Triangle", 4);
        oscType.setJustificationType(Justification::centred);
        addAndMakeVisible(&oscType);
        oscTypeAttach.reset(new ComboBoxAttachment(parameters, "fm_oscType", oscType));

        // Sliders
        multiSlider.setLookAndFeel(&customSlider);
        multiSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        multiSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        //multiSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&multiSlider);
        multiAttach.reset(new SliderAttachment(parameters, "fm_multi", multiSlider));

        freqSlider.setLookAndFeel(&customSlider);
        freqSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        freqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        freqSlider.setTextValueSuffix(" Hz");
        //freqSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&freqSlider);
        freqAttach.reset(new SliderAttachment(parameters, "fm_freq", freqSlider));

        depthSlider.setLookAndFeel(&customSlider);
        depthSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        depthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        //depthSlider.textFromValueFunction(); // use a lambda to alter the text
        //depthSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&depthSlider);
        depthAttach.reset(new SliderAttachment(parameters, "fm_depth", depthSlider));

        // Labels
        fmLabel.setText("FM", dontSendNotification);
        fmLabel.setFont(Font(20.0f, Font::bold));
        fmLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&fmLabel);

        multiLabel.setText("Multi", dontSendNotification);
        multiLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&multiLabel);
        multiLabel.attachToComponent(&multiSlider, false);

        freqLabel.setText("Freq", dontSendNotification);
        freqLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&freqLabel);
        freqLabel.attachToComponent(&freqSlider, false);

        depthLabel.setText("Depth", dontSendNotification);
        depthLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&depthLabel);
        depthLabel.attachToComponent(&depthSlider, false);
    }

    ~FmGUI()
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
        fmLabel.setBounds(area.removeFromLeft(boxWidth).removeFromTop(rotaryHeight / 2));
        Rectangle<int> boxArea = area.removeFromRight(boxWidth * 2).removeFromTop(rotaryHeight / 2);
        oscType.setBounds(boxArea.getX(), boxArea.getY() + boxArea.getHeight() / 2 - boxHeight / 2,
            boxArea.getWidth(), boxHeight);

        // bottom half
        area = getLocalBounds().reduced(padding);
        area.removeFromBottom(padding);
        multiSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        freqSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        depthSlider.setBounds(area.removeFromBottom(rotaryHeight));
    }

private:
    AudioProcessorValueTreeState& parameters;

    ComboBox oscType;

    CustomSlider customSlider;
    CustomBox customBox;

    Slider multiSlider;
    PercentSlider depthSlider;
    Slider freqSlider;

    Label multiLabel;
    Label depthLabel;
    Label freqLabel;
    Label fmLabel;

    std::unique_ptr<SliderAttachment> multiAttach;
    std::unique_ptr<SliderAttachment> depthAttach;
    std::unique_ptr<SliderAttachment> freqAttach;
    std::unique_ptr<ComboBoxAttachment> oscTypeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmGUI)
};
