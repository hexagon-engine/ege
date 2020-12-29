/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace EGE
{

bool operator==(const sf::RenderStates& _1, const sf::RenderStates& _2);

class RenderStates
{
public:

    sf::RenderStates& sfStates() { return m_states; }
    const sf::RenderStates& sfStates() const { return m_states; }

    bool operator==(const RenderStates& states) const { return states.m_states == m_states; }
    bool operator!=(const RenderStates& states) const { return !(states == *this); }

private:
    sf::RenderStates m_states;
};

}
