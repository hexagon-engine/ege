/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Client.h"

#include <iostream>

namespace EGE
{

bool Client::connect(sf::IpAddress ip, unsigned short port)
{
    sf::Lock lock(m_accessMutex);

    m_socket = std::make_shared<sf::TcpSocket>();
    sf::Socket::Status status = m_socket->connect(ip, port);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0019 EGE/network: Client: Connection failed to (" << ip << ":" << port << ")" << std::endl;
        return false;
    }
    return true;
}

void Client::update()
{
    sf::Packet packet;

    if(m_socket->receive(packet) == sf::Socket::Done)
    {
        onReceive(makePacket(packet));
    }
    else
    {
        disconnect();
    }
}

}
