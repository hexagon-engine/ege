/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CheckBox.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class RadioButton : public CheckBox
{
public:
    explicit RadioButton(Widget& parent)
    : CheckBox(parent) {}

protected:
    virtual void render(Renderer& renderer) const override;
    virtual void updateGeometry(Renderer& renderer) override;

private:
    virtual void onClick(EGE::Vec2d);
};

}
