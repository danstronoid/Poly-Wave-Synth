/*
  ==============================================================================

    CustomSliderLook.h
    Created: 24 Mar 2020 11:24:22am
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ColorPalette.h"

class CustomSlider : public LookAndFeel_V4
{
public:
    CustomSlider()
    {
        // slider
        setColour(Slider::backgroundColourId, palette.background);
        setColour(Slider::rotarySliderOutlineColourId, palette.background);
        setColour(Slider::trackColourId, palette.accent);
        setColour(Slider::rotarySliderFillColourId, palette.accent);
        setColour(Slider::thumbColourId, palette.foreground);

        // text box
        setColour(Slider::textBoxTextColourId, palette.textBright);
        setColour(Slider::textBoxBackgroundColourId, palette.transparent);
        setColour(Slider::textBoxOutlineColourId, palette.transparent);

        // highlighted text box
        setColour(Slider::textBoxHighlightColourId, palette.accent);
        setColour(TextEditor::highlightedTextColourId, palette.textDark);
        setColour(Label::backgroundWhenEditingColourId, palette.transparent);
        setColour(Label::outlineWhenEditingColourId, palette.transparent);
        setColour(Label::textWhenEditingColourId, palette.accent);
    }

    // Draws a linear vertical or horizontal sliders.  This does not implement multi point sliders.
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/, const Slider::SliderStyle /*style*/, Slider& slider) override
    {
        Point<float> startPoint(slider.isHorizontal() ? x : x + width * 0.5f,
                                slider.isHorizontal() ? y + height * 0.5f : height + y);

        Point<float> endPoint(slider.isHorizontal() ? width + x : startPoint.x,
                              slider.isHorizontal() ? startPoint.y : y);

        Point<float> thumbPoint(slider.isHorizontal() ? sliderPos : (x + width * 0.5f),
                                slider.isHorizontal() ? (y + height * 0.5f) : sliderPos);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setColour(slider.findColour(Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::square });

        Path valueTrack;
        valueTrack.startNewSubPath(startPoint);
        valueTrack.lineTo(thumbPoint);
        g.setColour(slider.findColour(Slider::trackColourId));
        g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::square });

        g.setColour(slider.findColour(Slider::thumbColourId));
        if (slider.isHorizontal())
            g.fillRoundedRectangle(Rectangle<float>(thumbHeight, thumbWidth).withCentre(thumbPoint), 0.0f);
        else
            g.fillRoundedRectangle(Rectangle<float>(thumbWidth, thumbHeight).withCentre(thumbPoint), 0.0f);
    }


    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        Rectangle<float> bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        float radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float arcRadius = radius - trackWidth * 0.5f;

        Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 
            0.0f, rotaryStartAngle, rotaryEndAngle, true);

        g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
        g.strokePath(backgroundArc, PathStrokeType(trackWidth, PathStrokeType::curved, PathStrokeType::square));


        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 
            0.0f, rotaryStartAngle, angle, true);

        g.setColour(slider.findColour(Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, PathStrokeType(trackWidth, PathStrokeType::curved, PathStrokeType::square));

        Path thumb;
        thumb.addRectangle(-thumbHeight * 0.5f, -arcRadius - thumbWidth * 0.5f, thumbHeight, thumbWidth);
        thumb.applyTransform(AffineTransform::rotation(angle).translated(bounds.getCentreX(), bounds.getCentreY()));

        g.setColour(slider.findColour(Slider::thumbColourId));
        g.fillPath(thumb);
    }

    // This implementation provides custom textbox locations
    Slider::SliderLayout getSliderLayout(Slider& slider) override
    {
        Slider::SliderLayout layout;

        Rectangle<int> bounds = slider.getLocalBounds();
        layout.sliderBounds = bounds;

        int thumbPadding = (int)thumbHeight * 2;
        if (slider.isHorizontal())
            layout.sliderBounds.reduce(thumbPadding, 0);
        else if (slider.isVertical())
            layout.sliderBounds.reduce(0, thumbPadding);

        if (slider.getTextBoxPosition() != Slider::NoTextBox)
        {
            int textBoxWidth = slider.getTextBoxWidth();
            int textBoxHeight = 20;

            layout.textBoxBounds.setWidth(textBoxWidth);
            layout.textBoxBounds.setHeight(textBoxHeight);

            // put the text box in the center of the rotary, or put it below
            if (slider.isRotary())
            {
                if (slider.getTextBoxPosition() == Slider::TextBoxBelow) 
                {
                    layout.textBoxBounds.setX((bounds.getWidth() - textBoxWidth) / 2);
                    layout.textBoxBounds.setY(bounds.getHeight() - textBoxHeight);
                    layout.sliderBounds.removeFromBottom(textBoxHeight);
                }
                else
                {
                    layout.textBoxBounds.setX(bounds.getCentreX() - textBoxWidth / 2);
                    layout.textBoxBounds.setY(bounds.getCentreY() - textBoxHeight / 2);
                }       
            }
            // put the text box to the left of a horizontal slider
            else if (slider.isHorizontal())
            {
                layout.textBoxBounds.setX(0);
                layout.textBoxBounds.setY((bounds.getHeight() - textBoxHeight) / 2);
                layout.sliderBounds.removeFromLeft(textBoxWidth);
            }
            // put the text box below anything else
            else
            {
                layout.textBoxBounds.setX((bounds.getWidth() - textBoxWidth) / 2);
                layout.textBoxBounds.setY(bounds.getHeight() - textBoxHeight);
                layout.sliderBounds.removeFromBottom(textBoxHeight);
            }
        }

        return layout;
    }

private:

    // Color palette
    ColorPalette palette;

    const float trackWidth = 6.0f;
    const float thumbWidth = 18.0f;
    const float thumbHeight = 6.0f;
};