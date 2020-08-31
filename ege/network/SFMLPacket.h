/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Network.hpp>

#include "Packet.h"

namespace EGE
{

// abstract
class SFMLPacket : public Packet
{
public:
    virtual sf::Packet toSFMLPacket() = 0;
};

}
