/*
  ==============================================================================

    CustomBox.h
    Created: 26 Mar 2020 4:45:59pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CustomBox : public LookAndFeel_V4
{
public:
    CustomBox()
    {
        // Color palette
        Colour background = Colours::darkgrey;
        Colour foreground = Colours::grey;
        Colour bright = Colours::ghostwhite;
        Colour accent = Colours::darkorange.darker(0.1f);
        Colour transparent = Colours::transparentWhite;

        // init colors
        setColour(ComboBox::backgroundColourId, background);
        setColour(ComboBox::outlineColourId, transparent);
        setColour(ComboBox::arrowColourId, bright);
        setColour(ComboBox::textColourId, bright);

        setColour(PopupMenu::backgroundColourId, background);
        setColour(PopupMenu::textColourId, bright);
        setColour(PopupMenu::highlightedBackgroundColourId, accent);
    }

    void drawComboBox(Graphics& g, int width, int height, bool,
        int, int, int, int, ComboBox& box) override
    {
        Rectangle<int> boxBounds(0, 0, width, height);

        g.setColour(box.findColour(ComboBox::backgroundColourId));
        g.fillRect(boxBounds.toFloat());

        g.setColour(box.findColour(ComboBox::outlineColourId));
        g.drawRect(boxBounds.toFloat().reduced(0.5f, 0.5f), 1.0f);

        Rectangle<int> arrowZone(width - 30, 0, 20, height);
        Path path;
        path.startNewSubPath(arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
        path.lineTo(static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
        path.lineTo(arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

        g.setColour(box.findColour(ComboBox::arrowColourId));
        g.strokePath(path, PathStrokeType(2.0f));
    }

private:

};
