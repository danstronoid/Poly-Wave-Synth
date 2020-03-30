/*
  ==============================================================================

    ColorPalette.h
    Created: 28 Mar 2020 10:40:03am
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// a collection of colors to be used throughout
struct ColorPalette
{
    Colour window = Colours::darkgrey.darker(1);
    Colour background = Colours::darkgrey;
    Colour foreground = Colours::grey.brighter();
    Colour accent = Colours::deepskyblue;//Colours::darkorange.darker(0.1f);
    Colour textBright = Colours::ghostwhite;
    Colour textDark = Colours::black;
    Colour transparent = Colours::transparentWhite;
};