/*
  ==============================================================================

    DecibelSlider.h
    Created: 30 Mar 2020 9:20:41am
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
		String decibelText = text.upToFirstOccurrenceOf("dB",false, false).trim();
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