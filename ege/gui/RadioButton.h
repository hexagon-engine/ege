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

    virtual void renderOnly(Renderer& renderer);

private:
    virtual void onClick(EGE::Vec2d);
};

}
