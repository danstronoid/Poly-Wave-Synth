/*
  ==============================================================================

    FilterGUI.h
    Created: 18 Feb 2020 4:33:31pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class FilterGUI    : public Component
{
public:
    FilterGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        cutoffSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        cutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&cutoffSlider);
        cutoffAttach.reset(new SliderAttachment(parameters, "cutoff", cutoffSlider));
        cutoffLabel.setText("Cutoff", dontSendNotification);
        cutoffLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&cutoffLabel);
        cutoffLabel.attachToComponent(&cutoffSlider, false);

        qSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&qSlider);
        qAttach.reset(new SliderAttachment(parameters, "q", qSlider));
        qLabel.setText("Q", dontSendNotification);
        qLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&qLabel);
        qLabel.attachToComponent(&qSlider, false);

    }

    ~FilterGUI()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (Colours::white);
        //g.setFont (14.0f);
        //g.drawText ("FilterGUI", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        const int padding = 20;
        Rectangle<int> area = getLocalBounds();
        int sliderWidth = area.getWidth() / 2;
        int sliderHeight = area.getHeight() / 2;
        //remove space for the label
        area.removeFromTop(padding * 2);

        cutoffSlider.setBounds(area.removeFromRight(sliderWidth).removeFromTop(sliderHeight));
        qSlider.setBounds(area.removeFromRight(sliderWidth).removeFromTop(sliderHeight));

    }

private:
    AudioProcessorValueTreeState& parameters;

    Slider cutoffSlider;
    Slider qSlider;

    Label cutoffLabel;
    Label qLabel;

    std::unique_ptr<SliderAttachment> cutoffAttach;
    std::unique_ptr<SliderAttachment> qAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGUI)
};
