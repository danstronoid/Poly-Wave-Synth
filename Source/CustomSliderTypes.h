/*
  ==============================================================================

    CustomSliderTypes.h
    Created: 3 Apr 2020 3:00:01pm
    Author:  Daniel Schwartz

	This file defines a couple of custom slider implementations to override the
	ValueFromText and TextFromValue functions.  

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


//============================================================================== 

// use this type of slider to display values in dB scale

class DecibelSlider : public Slider
{
public:
	DecibelSlider() : m_minusInfdB(-56.0)
	{
	}

	double getValueFromText(const String& text) override
	{
		String decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();
		double valueDB = decibelText.equalsIgnoreCase("-INF") ? m_minusInfdB : decibelText.getDoubleValue();
		return Decibels::decibelsToGain(valueDB, m_minusInfdB);
	}

	String getTextFromValue(double value) override
	{
		double valueDB = Decibels::gainToDecibels(value, m_minusInfdB);
		return Decibels::toString(valueDB, 1, m_minusInfdB);
	}

	double getMinusInf() const
	{
		return m_minusInfdB;
	}

	void setMinusInf(double minusInf)
	{
		m_minusInfdB = minusInf;
	}
private:
	double m_minusInfdB;
};

//============================================================================== 

// use this slider to display values as a percentage

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

//============================================================================== 

// use this slider to display the frequency of a fixed osclillator
// the advantage of using this instead of just changing the suffix 
// is that it shows "20 Hz" as being "Off" which is true for the way fixed Oscillators 
// are implemented in this synth.

class OscFreqSlider : public Slider
{
public:
	OscFreqSlider() {}

	double getValueFromText(const String& text) override
	{
		if (text == "Off")
			return 20;
		else
		{
			String freqText = text.upToFirstOccurrenceOf("Hz", false, false).trim();
			return freqText.getFloatValue();
		}
	}

	String getTextFromValue(double value) override
	{
		if (value == 20)
			return String("Off");
		else
			return String(value) + " Hz";
	}
};