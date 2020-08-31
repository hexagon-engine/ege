/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "NetworkEndpoint.h"

#include <iostream>

namespace EGE
{

void NetworkEndpoint::disconnect()
{
    if(!m_connected)
        return;
    m_connected = false;

    std::cerr << "001A EGE/network: Disconnecting network endpoint: " << m_socket->getLocalPort() << std::endl;
    m_socket->disconnect();
}

}
