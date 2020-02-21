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
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

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
        cutoffSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        cutoffSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&cutoffSlider);
        cutoffAttach.reset(new SliderAttachment(parameters, "cutoff", cutoffSlider));
        cutoffLabel.setText("Cutoff", dontSendNotification);
        cutoffLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&cutoffLabel);
        cutoffLabel.attachToComponent(&cutoffSlider, false);

        qSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        qSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0); 
        qSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&qSlider);
        qAttach.reset(new SliderAttachment(parameters, "q", qSlider));
        qLabel.setText("Q", dontSendNotification);
        qLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&qLabel);
        qLabel.attachToComponent(&qSlider, false);

        envAmtSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        envAmtSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        envAmtSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&envAmtSlider);
        envAmtAttach.reset(new SliderAttachment(parameters, "envAmt", envAmtSlider));
        envAmtLabel.setText("Env", dontSendNotification);
        envAmtLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&qLabel);
        envAmtLabel.attachToComponent(&envAmtSlider, false);

        filterType.addItem("Low Pass", 1); 
        filterType.addItem("High Pass", 2);
        filterType.addItem("Band Pass", 3);
        addAndMakeVisible(&filterType);
        filterTypeAttach.reset(new ComboBoxAttachment(parameters, "filterType", filterType));
        filterTypeLabel.setText("Filter Type", dontSendNotification);
        filterTypeLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&filterTypeLabel);
        filterTypeLabel.attachToComponent(&filterType, false);
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
        const int padding = 10;
        Rectangle<int> area = getLocalBounds().reduced(padding);
        int sliderWidth = area.getWidth() / 3;
        int sliderHeight = area.getHeight() / 3;
        //remove space for the label
        area.removeFromTop(padding * 2);

        cutoffSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
        qSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
        envAmtSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));

        area = getLocalBounds().reduced(padding);
        area.removeFromTop(sliderHeight + padding * 4);
        filterType.setBounds(area.removeFromLeft(sliderWidth * 2).removeFromTop(padding * 2));
        
    }

private:
    AudioProcessorValueTreeState& parameters;

    Slider cutoffSlider;
    Slider qSlider;
    Slider envAmtSlider;
    ComboBox filterType;

    Label cutoffLabel;
    Label qLabel;
    Label envAmtLabel;
    Label filterTypeLabel;

    std::unique_ptr<SliderAttachment> cutoffAttach;
    std::unique_ptr<SliderAttachment> qAttach;
    std::unique_ptr<SliderAttachment> envAmtAttach;
    std::unique_ptr<ComboBoxAttachment> filterTypeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGUI)
};
