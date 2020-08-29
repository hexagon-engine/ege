/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Client.h"

#include "Server.h"

namespace EGE
{

bool ClientConnection::send(std::shared_ptr<Packet> packet)
{
    sf::Packet sfPacket = packet->toSFMLPacket();
    sf::Socket::Status status = m_socket->send(sfPacket);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0017 EGE/network: Socket Send failed (system error)" << std::endl;
        kick();
        return false;
    }
    return true;
}

std::shared_ptr<Packet> ClientConnection::receive()
{
    sf::Packet sfPacket;
    sf::Socket::Status status = m_socket->receive(sfPacket);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0016 EGE/network: Socket Receive failed (system error)" << std::endl;
        kick();
        return nullptr;
    }
    return makePacket(sfPacket);
}

void ClientConnection::kick()
{
    m_socket->disconnect();
    m_connected = false;
}

}
