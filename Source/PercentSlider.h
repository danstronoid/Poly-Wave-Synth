/*
  ==============================================================================

    PercentSlider.h
    Created: 30 Mar 2020 12:39:13pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class PercentSlider : public Slider
{
public:
    PercentSlider() {}

    double getValueFromText(const String& text) override
    {
        String percText = text.upToFirstOccurrenceOf("%", false, false).trim();
        return percText.getFloatValue() / 100.0f;
    }

    String getTextFromValue(double value) override
    {
        int percValue = roundToInt(value * 100.0);
        return String(percValue) + "%";
    }
};