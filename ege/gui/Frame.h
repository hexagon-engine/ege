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

    virtual sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect();
    }

protected:
    virtual void render(Renderer& renderer) const override;

private:
    sf::String m_label;
};

}
