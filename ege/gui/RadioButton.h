/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CheckBox.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

class RadioButton : public CheckBox
{
public:
    RadioButton(Widget* parent)
    : CheckBox(parent) {}

    virtual void renderOnly(sf::RenderTarget& target);

private:
    virtual void onClick(sf::Vector2f);
};

}
