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

class Frame : public Widget
{
public:
    explicit Frame(Widget& parent)
    : Widget(parent) {}

    void setLabel(sf::String label)
    {
        m_label = label;
    }

    virtual void renderOnly(Renderer& renderer) override;
    virtual sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect();
    }

private:
    sf::String m_label;
};

}
