/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Button : public Widget
{
public:
    class Command : public Widget::Command
    {
    public:
        Command(Button* button)
        : Widget::Command("EGE::Button::Command"), m_button(button)
        {}

        Button* getButton() const
        {
            return m_button;
        }
    private:
        Button* m_button;
    };

    Button(Widget* parent)
    : Widget(parent) {}

    void setLabel(sf::String label)
    {
        m_label = label;
    }
    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event);
    virtual void onTouchEnd(sf::Event::TouchEvent& event) override;
    virtual void renderOnly(sf::RenderTarget& target) override;

private:
    // position may be needed for some animations
    void handleClick(sf::Vector2f position);

    sf::Vector2f m_lastClickPos;
    sf::String m_label;
};

}
