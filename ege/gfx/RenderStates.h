/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace EGE
{

class RenderStates
{
public:

    sf::RenderStates& sfStates()
    {
        return m_states;
    }

    const sf::RenderStates& sfStates() const
    {
        return m_states;
    }

private:
    sf::RenderStates m_states;
};

}
