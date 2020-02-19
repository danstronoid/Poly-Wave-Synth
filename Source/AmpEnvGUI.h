/*
  ==============================================================================

    AmpEnvGUI.h
    Created: 18 Feb 2020 3:04:39pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class AmpEnvGUI    : public Component
{
public:
    AmpEnvGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        attackSlider.setSliderStyle(Slider::LinearVertical);
        attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, getWidth(), 20); 
        addAndMakeVisible(&attackSlider);
        attackAttach.reset(new SliderAttachment(parameters, "amp_attack", attackSlider));
        attackLabel.setText("A", dontSendNotification);
        attackLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&attackLabel);
        attackLabel.attachToComponent(&attackSlider, false);

        decaySlider.setSliderStyle(Slider::LinearVertical);
        decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, getWidth(), 20);
        addAndMakeVisible(&decaySlider);
        decayAttach.reset(new SliderAttachment(parameters, "amp_decay", decaySlider));
        decayLabel.setText("D", dontSendNotification);
        decayLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&decayLabel);
        decayLabel.attachToComponent(&decaySlider, false);

        sustainSlider.setSliderStyle(Slider::LinearVertical);
        sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, getWidth(), 20);
        addAndMakeVisible(&sustainSlider);
        sustainAttach.reset(new SliderAttachment(parameters, "amp_sustain", sustainSlider));
        sustainLabel.setText("S", dontSendNotification);
        sustainLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&sustainLabel);
        sustainLabel.attachToComponent(&sustainSlider, false);

        releaseSlider.setSliderStyle(Slider::LinearVertical);
        releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, getWidth(), 20);
        addAndMakeVisible(&releaseSlider);
        releaseAttach.reset(new SliderAttachment(parameters, "amp_release", releaseSlider));
        releaseLabel.setText("R", dontSendNotification);
        releaseLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&releaseLabel);
        releaseLabel.attachToComponent(&releaseSlider, false);
    }

    ~AmpEnvGUI()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //g.setColour (Colours::white);
        //g.setFont (16.0f);
        //g.drawText("AMP", getLocalBounds().removeFromTop(20), Justification::centred, true);
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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpEnvGUI)
};
