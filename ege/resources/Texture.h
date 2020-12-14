/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Vector.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Texture
{
public:
    String getName() { return m_name; }
    virtual sf::Texture& getTexture() = 0;

protected:
    void setName(String s) { m_name = s; }

private:
    String m_name = "NoName";
};

}
