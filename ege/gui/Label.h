/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Label : public Widget
{
public:
    enum class Align
    {
        Left,
        Center,
        Right
    };

    Label(Widget* parent)
    : Widget(parent) {}

    void setString(sf::String str)
    {
        m_string = str;
    }
    void setTextAlign(Align align);
    // do not confuse with setPosition
    void setTextPosition(sf::Vector2f position);
    void setFontSize(int size);
    virtual void render(sf::RenderTarget& target) override;

private:
    virtual void setPosition(sf::Vector2f position)
    {
        Widget::setPosition(position);
    };
    void updateGeometry();

    sf::String m_string;
    Align m_align = Align::Left;
    int m_fontSize = 15;
    sf::Text m_text;
    sf::Vector2f m_textPosition;
    std::shared_ptr<sf::Font> m_font;
    bool m_geometryChanged = true;
};

}
