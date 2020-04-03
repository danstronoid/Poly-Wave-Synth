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
    void resized() override {}

    // update the level value, call this in timerCallback
    void update(float level);

    // set colors
    void setBackgroundColor(const Colour& color) { m_bgColor = color; }
    void setFillColor(const Colour& color) { m_fillColor = color; }
    void setPeakColor(const Colour& color) { m_peakColor = color; }

    // set the meter to fill the entire area
    void setFillArea(bool fillArea) { m_fillArea = fillArea; }

private:
    // initialize with some default colors
    Colour m_bgColor{ Colours::darkgrey };
    Colour m_fillColor{ Colours::ghostwhite };
    Colour m_peakColor{ Colours::grey };

    bool m_fillArea{ false };

    float m_level{ 0.0f };
    float m_peak{ 0.0f };

    int m_counter{ 0 }; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
