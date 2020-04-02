/*
  ==============================================================================

    LevelMeter.h
    Created: 2 Apr 2020 10:45:04am
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LevelMeter    : public Component
{
public:
    LevelMeter();
    ~LevelMeter();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
