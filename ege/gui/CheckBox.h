/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

class CheckBox : public Button
{
public:
    CheckBox(Widget* parent)
    : Button(parent) {}

    void setChecked(bool checked = true)
    {
        m_checked = checked;
    }
    bool isChecked()
    {
        return m_checked;
    }
    void setLabel(sf::String label)
    {
        Button::setLabel(label);
        m_geometryUpdate = true;
    }

    virtual void renderOnly(sf::RenderTarget& target);

protected:
    bool m_geometryUpdate = true;
    bool m_checked = false;

    virtual void setSize(sf::Vector2f) {}
    virtual void onClick(sf::Vector2f);
};

}
