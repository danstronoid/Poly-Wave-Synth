/*
  ==============================================================================

    CustomBox.h
    Created: 26 Mar 2020 4:45:59pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ColorPalette.h"

class CustomBox : public LookAndFeel_V4
{
public:
    CustomBox()
    {
        // Color palette
        ColorPalette palette;

        // init colors
        setColour(ComboBox::backgroundColourId, palette.background);
        setColour(ComboBox::outlineColourId, palette.transparent);
        setColour(ComboBox::arrowColourId, palette.textBright);
        setColour(ComboBox::textColourId, palette.textBright);

        setColour(PopupMenu::backgroundColourId, palette.background);
        setColour(PopupMenu::textColourId, palette.textBright);
        setColour(PopupMenu::highlightedTextColourId, palette.textDark);
        setColour(PopupMenu::highlightedBackgroundColourId, palette.accent);
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
