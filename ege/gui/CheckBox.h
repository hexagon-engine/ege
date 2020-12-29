/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Button.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class CheckBox : public Button
{
public:
    explicit CheckBox(Widget& parent)
    : Button(parent) {}

    void setChecked(bool checked = true)
    {
        m_checked = checked;
    }
    bool isChecked() const
    {
        return m_checked;
    }
    void setLabel(sf::String label)
    {
        Button::setLabel(label);
        setGeometryNeedUpdate();
    }

protected:
    bool m_checked = false;

    virtual void render(Renderer& renderer) const override;
    virtual void updateGeometry(Renderer& renderer) override;

    virtual void setSize(EGE::Vec2d) {}
    virtual void onClick(EGE::Vec2d);
};

}
