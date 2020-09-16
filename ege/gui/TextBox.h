/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class TextBox : public Widget
{
public:
    TextBox(Widget* parent);

    void setText(sf::String text)
    {
        m_text = text;
    }
    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    virtual void onTextEnter(sf::Event::TextEvent& event);
    virtual void onKeyPress(sf::Event::KeyEvent& event);
    virtual void renderOnly(sf::RenderTarget& target) override;

private:
    sf::String m_text;
    size_t m_caretPos = 0;
    bool m_caretShown = true;
};

}
