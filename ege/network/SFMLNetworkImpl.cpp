/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SFMLNetworkImpl.h"

#include <iostream>

namespace EGE
{

bool SFMLNetworkImpl::sendTo(NetworkEndpoint* endpoint, std::shared_ptr<Packet> packet)
{
    sf::Lock lock(endpoint->getMutex());
    SFMLPacket* packet2 = (SFMLPacket*)packet.get();

    sf::Packet sfPacket = packet2->toSFMLPacket();
    sf::Socket::Status status = endpoint->getSocket().lock()->send(sfPacket);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0017 EGE/network: Socket Send failed (system error) to (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")" << std::endl;
        endpoint->disconnect();
        return false;
    }
    return true;
}

std::shared_ptr<Packet> SFMLNetworkImpl::receiveFrom(NetworkEndpoint* endpoint)
{
    sf::Lock lock(endpoint->getMutex());

    sf::Packet sfPacket;
    sf::Socket::Status status = endpoint->getSocket().lock()->receive(sfPacket);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0016 EGE/network: Socket Receive failed (system error) from (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")" << std::endl;
        endpoint->disconnect();
        return nullptr;
    }
    return makePacket(sfPacket);
}

}
