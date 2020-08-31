/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <SFML/Network.hpp>

#include "NetworkEndpoint.h"
#include "SFMLPacket.h"

namespace EGE
{

class SFMLNetworkImpl
{
public:
    virtual bool sendTo(NetworkEndpoint* endpoint, std::shared_ptr<Packet> packet);
    virtual std::shared_ptr<Packet> receiveFrom(NetworkEndpoint* endpoint);

    virtual std::shared_ptr<SFMLPacket> makePacket(sf::Packet& packet) = 0;
};

}
