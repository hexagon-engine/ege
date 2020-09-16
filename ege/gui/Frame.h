/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Frame : public Widget
{
public:
    Frame(Widget* parent)
    : Widget(parent) {}

    void setLabel(sf::String label)
    {
        m_label = label;
    }
    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    virtual void renderOnly(sf::RenderTarget& target) override;
    virtual sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect();
    }

private:
    sf::String m_label;
};

}
