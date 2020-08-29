/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Network.hpp>

namespace EGE
{

// abstract
class Packet
{
public:
    virtual bool fromSFMLPacket(const sf::Packet& packet) = 0;
    virtual const sf::Packet& toSFMLPacket() = 0;

};

}
