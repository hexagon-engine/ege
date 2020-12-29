/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gfx/RenderStates.h>
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

    explicit Button(Widget& parent)
    : Widget(parent) {}

    virtual void setLabel(sf::String label)
    {
        m_label = label;
    }

    sf::String getLabel() const
    {
        return m_label;
    }

    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event);
    virtual void onTouchEnd(sf::Event::TouchEvent& event) override;

    void setCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

protected:
    virtual void render(Renderer& renderer) const override;

    virtual void onClick(EGE::Vec2d position);
    std::function<void()> m_callback;

private:
    // position may be needed for some animations
    void handleClick(EGE::Vec2d position);

    EGE::Vec2d m_lastClickPos;
    sf::String m_label;
};

}
