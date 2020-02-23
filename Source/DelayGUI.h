/*
  ==============================================================================

    DelayGUI.h
    Created: 22 Feb 2020 9:33:59am
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DelayGUI    : public Component
{
public:
    DelayGUI(AudioProcessorValueTreeState& vts) :
        parameters(vts)
    {
        setSize(200, 200);

    }

    ~DelayGUI()
    {
    }

    void paint (Graphics& g) override
    {
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        //g.setColour (Colours::grey);
        //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }

    void resized() override
    {
        

    }

private:
    AudioProcessorValueTreeState& parameters;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayGUI)
};
