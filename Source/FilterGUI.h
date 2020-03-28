/*
  ==============================================================================

    FilterGUI.h
    Created: 18 Feb 2020 4:33:31pm
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
class FilterGUI    : public Component
{
public:
    FilterGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

        // Sliders
        cutoffSlider.setLookAndFeel(&customSlider);
        cutoffSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        cutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        cutoffSlider.setTextValueSuffix(" Hz");
        //cutoffSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&cutoffSlider);
        cutoffAttach.reset(new SliderAttachment(parameters, "filter_cutoff", cutoffSlider));

        qSlider.setLookAndFeel(&customSlider);
        qSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        //qSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&qSlider);
        qAttach.reset(new SliderAttachment(parameters, "filter_q", qSlider));

        envAmtSlider.setLookAndFeel(&customSlider);
        envAmtSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        envAmtSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        //envAmtSlider.setPopupDisplayEnabled(true, false, this);
        addAndMakeVisible(&envAmtSlider);
        envAmtAttach.reset(new SliderAttachment(parameters, "filter_envAmt", envAmtSlider));

        filterType.setLookAndFeel(&customBox);
        filterType.addItem("Low Pass", 1); 
        filterType.addItem("High Pass", 2);
        filterType.addItem("Band Pass", 3);
        filterType.setJustificationType(Justification::centred);
        addAndMakeVisible(&filterType);
        filterTypeAttach.reset(new ComboBoxAttachment(parameters, "filter_type", filterType));

        // Labels
        cutoffLabel.setText("Cutoff", dontSendNotification);
        cutoffLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&cutoffLabel);
        cutoffLabel.attachToComponent(&cutoffSlider, false);

        qLabel.setText("Q", dontSendNotification);
        qLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&qLabel);
        qLabel.attachToComponent(&qSlider, false);

        envAmtLabel.setText("Env", dontSendNotification);
        envAmtLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&envAmtLabel);
        envAmtLabel.attachToComponent(&envAmtSlider, false);

        filterLabel.setText("Filter", dontSendNotification);
        filterLabel.setFont(Font(20.0f, Font::bold));
        filterLabel.setJustificationType(Justification::centred);
        addAndMakeVisible(&filterLabel);
    }

    ~FilterGUI()
    {
    }

    void paint (Graphics& /*g*/) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (Colours::white);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        //Rectangle<int> area = getLocalBounds().reduced(padding);
        //g.setColour (Colours::white);
        //g.setFont (Font(Font::getDefaultSansSerifFontName(), 42.0f, Font::bold));
        //g.drawText ("Filter", area.removeFromTop(getHeight() / 3), Justification::left, true);
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
        filterLabel.setBounds(area.removeFromLeft(boxWidth).removeFromTop(rotaryHeight / 2));
        Rectangle<int> boxArea = area.removeFromLeft(boxWidth * 2).removeFromTop(rotaryHeight / 2);
        filterType.setBounds(boxArea.getX(), boxArea.getY() + boxArea.getHeight() / 2 - boxHeight / 2,
            boxArea.getWidth(), boxHeight);

        // bottom half
        area = getLocalBounds().reduced(padding);
        area.removeFromBottom(padding);
        cutoffSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        qSlider.setBounds(area.removeFromLeft(rotaryWidth).removeFromBottom(rotaryHeight));
        envAmtSlider.setBounds(area.removeFromBottom(rotaryHeight));

        
    }

private:
    AudioProcessorValueTreeState& parameters;

    CustomSlider customSlider;
    CustomBox customBox;

    Slider cutoffSlider;
    Slider qSlider;
    Slider envAmtSlider;
    ComboBox filterType;

    Label cutoffLabel;
    Label qLabel;
    Label envAmtLabel;
    Label filterLabel;

    std::unique_ptr<SliderAttachment> cutoffAttach;
    std::unique_ptr<SliderAttachment> qAttach;
    std::unique_ptr<SliderAttachment> envAmtAttach;
    std::unique_ptr<ComboBoxAttachment> filterTypeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGUI)
};
