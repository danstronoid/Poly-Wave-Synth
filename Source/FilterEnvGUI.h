/*
  ==============================================================================

    FilterEnvGUI.h
    Created: 18 Feb 2020 4:31:19pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomSlider.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class FilterEnvGUI    : public Component
{
public:
    FilterEnvGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        // Sliders
        attackSlider.setLookAndFeel(&customSlider);
        attackSlider.setSliderStyle(Slider::LinearVertical);
        attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
        //attackSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&attackSlider);
        attackAttach.reset(new SliderAttachment(parameters, "filterEnv_attack", attackSlider));

        decaySlider.setLookAndFeel(&customSlider);
        decaySlider.setSliderStyle(Slider::LinearVertical);
        decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
        //decaySlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&decaySlider);
        decayAttach.reset(new SliderAttachment(parameters, "filterEnv_decay", decaySlider));

        sustainSlider.setLookAndFeel(&customSlider);
        sustainSlider.setSliderStyle(Slider::LinearVertical);
        sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
        //sustainSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&sustainSlider);
        sustainAttach.reset(new SliderAttachment(parameters, "filterEnv_sustain", sustainSlider));

        releaseSlider.setLookAndFeel(&customSlider);
        releaseSlider.setSliderStyle(Slider::LinearVertical);
        releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
        //releaseSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&releaseSlider);
        releaseAttach.reset(new SliderAttachment(parameters, "filterEnv_release", releaseSlider));

        // Labels
        attackLabel.setText("A", dontSendNotification);
        attackLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&attackLabel);
        attackLabel.attachToComponent(&attackSlider, false);

        decayLabel.setText("D", dontSendNotification);
        decayLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&decayLabel);
        decayLabel.attachToComponent(&decaySlider, false);

        sustainLabel.setText("S", dontSendNotification);
        sustainLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&sustainLabel);
        sustainLabel.attachToComponent(&sustainSlider, false);

        releaseLabel.setText("R", dontSendNotification);
        releaseLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&releaseLabel);
        releaseLabel.attachToComponent(&releaseSlider, false);
    }

    ~FilterEnvGUI()
    {
    }

    void paint (Graphics& /*g*/) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (Colours::white);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    }

    void resized() override
    {
        const int padding = 20;
        Rectangle<int> area = getLocalBounds().reduced(padding);

        //remove space for the label
        area.removeFromTop(padding);

        int sliderWidth = area.getWidth() / 4;

        attackSlider.setBounds(area.removeFromLeft(sliderWidth));
        decaySlider.setBounds(area.removeFromLeft(sliderWidth));
        sustainSlider.setBounds(area.removeFromLeft(sliderWidth));
        releaseSlider.setBounds(area.removeFromLeft(sliderWidth));
    }

private:
    AudioProcessorValueTreeState& parameters;

    CustomSlider customSlider;

    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;

    Label attackLabel;
    Label decayLabel;
    Label sustainLabel;
    Label releaseLabel;

    std::unique_ptr<SliderAttachment> attackAttach;
    std::unique_ptr<SliderAttachment> decayAttach;
    std::unique_ptr<SliderAttachment> sustainAttach;
    std::unique_ptr<SliderAttachment> releaseAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterEnvGUI)
};
