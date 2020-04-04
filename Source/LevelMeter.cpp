/*
  ==============================================================================

    LevelMeter.cpp
    Created: 2 Apr 2020 10:45:04am
    Author:  Daniel Schwartz

    This draws a simple Level Meter.  This class doesn't have a timer, but rather
    the update and repaint functions should be called in the pluginEditor using
    a timerCallback.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LevelMeter.h"

//==============================================================================
LevelMeter::LevelMeter()
{
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (Graphics& g)
{
    Rectangle<float> area = g.getClipBounds().toFloat();
    Rectangle<float> peakArea;

    Path pFill;
    Path pBG;

    Point<float> minLevel;
    Point<float> maxLevel;
    Point<float> level;
    Point<float> peakLevel;

    float strokeWidth = 12.0f;
    float peakSize = 6.0f;

    //use a vertical meter if the height is greater than the width
    if (area.getHeight() >= area.getWidth())
    {
        if (m_fillArea)
            strokeWidth = area.getWidth();

        float xPos = area.getX() + area.getWidth() / 2;
        peakArea.setSize(strokeWidth, peakSize);

        minLevel.setXY(xPos, area.getHeight());
        maxLevel.setXY(xPos, area.getY());

        level.setXY(xPos, jmin<float>(area.getHeight() * (1 - m_level), area.getY()));

        peakLevel.setXY(xPos, jlimit<float>(area.getHeight() - peakSize / 2, 
            area.getY() + peakSize / 2, area.getHeight() * (1 - m_peak)));
    }
    else // otherwise use a horizontal meter
    {
        if (m_fillArea)
            strokeWidth = area.getHeight();

        float yPos = area.getY() + area.getHeight() / 2;
        peakArea.setSize(peakSize, strokeWidth);

        minLevel.setXY(area.getX(), yPos);
        maxLevel.setXY(area.getWidth(), yPos);

        level.setXY(jmin<float>(area.getWidth() * m_level, area.getWidth()), yPos);

        peakLevel.setXY(jlimit<float>(peakSize / 2, area.getWidth() - peakSize / 2, 
            area.getWidth() * m_peak), yPos);
    }

    // set paths
    PathStrokeType stroke(strokeWidth, PathStrokeType::beveled, PathStrokeType::butt);
    pBG.startNewSubPath(minLevel);
    pBG.lineTo(maxLevel);
    pFill.startNewSubPath(minLevel);
    pFill.lineTo(level);

    // draw background
    g.setColour(m_bgColor);
    g.strokePath(pBG, stroke);

    // draw level
    g.setColour(m_fillColor);
    g.strokePath(pFill, stroke);

    // draw peak
    if (m_peak >= 1.0f)
        g.setColour(Colours::red);
    else
        g.setColour(m_peakColor);

    g.fillRect(peakArea.withCentre(peakLevel));
}

void LevelMeter::update(float level)
{
    // set the peak level
    if (level > m_peak)
        m_peak = level;

    // update the level
    if (level > m_level)
        m_level = level;
    else if (level > 0.00001)
        m_level *= 0.95f;
    else
        m_level = 0.0f;

    // reset the peak every second
    if (++m_counter == 30)
    {
        m_peak = m_level;
        m_counter = 0;
    }
}

