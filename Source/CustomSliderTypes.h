/*
  ==============================================================================

    CustomSliderTypes.h
    Created: 3 Apr 2020 3:00:01pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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