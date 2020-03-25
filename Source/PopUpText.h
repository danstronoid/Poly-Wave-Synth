/*
  ==============================================================================

    PopUpText.h
    Created: 25 Feb 2020 4:39:59pm
    Author:  Daniel Schwartz

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PopUpText    : public Component
{
public:
    PopUpText(Component& component) :
        m_component(component)
    {

        text.addPopupMenuItems(menu, nullptr);
        menu.showMenuAsync(PopupMenu::Options());
    }

    ~PopUpText()
    {
    }

    void paint (Graphics& g) override
    {

    }

    void resized() override
    {
        

    }

private:
    PopupMenu menu;
    TextEditor text;

    Component& m_component;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopUpText)
};
