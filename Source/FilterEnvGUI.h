/*
  ==============================================================================

    FilterEnvGUI.h
    Created: 18 Feb 2020 4:31:19pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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

    }

    ~FilterEnvGUI()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("FilterEnvGUI", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    AudioProcessorValueTreeState& parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterEnvGUI)
};
