/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Client.h"

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <iostream>

namespace EGE
{

bool Client::connect(sf::IpAddress ip, unsigned short port)
{
    sf::Lock lock(m_accessMutex);

    m_socket = make<sf::TcpSocket>();
    sf::Socket::Status status = m_socket->connect(ip, port, sf::seconds(1.f));
    if(status != sf::Socket::Done)
    {
        err(LogLevel::Error) << "0019 EGE/network: Client: Connection failed to (" << ip << ":" << port << ")";
        return false;
    }
    return true;
}

void Client::update()
{
    auto packet = receive();

    if(packet)
    {
        onReceive(packet);
    }
    else
    {
        disconnect();
    }
}

}
