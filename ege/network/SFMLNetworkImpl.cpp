/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SFMLNetworkImpl.h"

#include <ege/debug/Logger.h>
#include <iostream>

namespace EGE
{

bool SFMLNetworkImpl::sendTo(NetworkEndpoint* endpoint, std::shared_ptr<Packet> packet)
{
    // Abort sending empty packet (or TO empty endpoint) with success state.
    if(!packet || !endpoint)
        return true;

    //sf::Lock lock(endpoint->getMutex());
    SFMLPacket* packet2 = (SFMLPacket*)packet.get();

    sf::Packet sfPacket = packet2->toSFMLPacket();
    sf::Socket::Status status = endpoint->getSocket().lock()->send(sfPacket);
    if(status != sf::Socket::Done)
    {
        err(LogLevel::Error) << "0017 EGE/network: Socket Send failed (system error) to (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")";
        //endpoint->disconnect();
        return false;
    }
    return true;
}

std::shared_ptr<Packet> SFMLNetworkImpl::receiveFrom(NetworkEndpoint* endpoint)
{
    // Abort receiving FROM empty endpoint with empty state.
    if(!endpoint)
        return nullptr;

    //sf::Lock lock(endpoint->getMutex());

    sf::Packet sfPacket;
    sf::Socket::Status status = endpoint->getSocket().lock()->receive(sfPacket);
    if(status != sf::Socket::Done)
    {
        err(LogLevel::Error) << "0016 EGE/network: Socket Receive failed (system error) from (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")";
        //endpoint->disconnect();
        return nullptr;
    }
    return makePacket(sfPacket);
}

}
