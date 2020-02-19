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

        oscType.addItem("Sine", 1);
        oscType.addItem("Square", 2);
        oscType.addItem("Saw", 3);
        oscType.addItem("Tri", 4);
        addAndMakeVisible(&oscType);
        oscTypeAttach.reset(new ComboBoxAttachment(parameters, "oscType", oscType));
        oscLabel.setText("Osc Type", dontSendNotification);
        addAndMakeVisible(&oscLabel);
        oscLabel.setJustificationType(Justification::centred);
        oscLabel.attachToComponent(&oscType, false);


        gainSlider.setSliderStyle(Slider::LinearVertical);
        gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, getWidth(), 20);
        addAndMakeVisible(&gainSlider);
        gainAttach.reset(new SliderAttachment(parameters, "gain", gainSlider));
        gainLabel.setText("Gain", dontSendNotification);
        gainLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&gainLabel);
        gainLabel.attachToComponent(&gainSlider, false);
    }

    ~OscGUI()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("OSC", getLocalBounds(), Justification::centred, true);
    }

    void resized() override
    {
        const int padding = 20;
        Rectangle<int> area = getLocalBounds().reduced(padding);

        //remove space for the label
        area.removeFromTop(padding);
     
        int sliderWidth = area.getWidth() / 4;

        gainSlider.setBounds(area.removeFromRight(sliderWidth));
        oscType.setBounds(area.removeFromTop(20));

    }

private:
    AudioProcessorValueTreeState& parameters;

    ComboBox oscType;
    Slider gainSlider;

    Label oscLabel;
    Label gainLabel;

    std::unique_ptr<ComboBoxAttachment> oscTypeAttach;
    std::unique_ptr<SliderAttachment> gainAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscGUI)
};
