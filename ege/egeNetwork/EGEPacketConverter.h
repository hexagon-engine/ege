/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Converter.h>

#include <SFML/Network.hpp>

namespace EGE
{

class EGEPacketConverter : public Converter<sf::Packet>
{
public:
    virtual bool in(sf::Packet& input, std::shared_ptr<Object>& object) const;
    virtual bool out(sf::Packet& output, const Object& object) const;
};

}
