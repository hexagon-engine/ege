/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include <ege/util/Converter.h>

#include <SFML/Network.hpp>

namespace EGE
{

class EGEPacketConverter : public Converter<sf::Packet>
{
public:
    virtual bool in(sf::Packet& input, ObjectMap& object) const;
    virtual bool out(sf::Packet& output, const ObjectMap& object) const;
};

}
