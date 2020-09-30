/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "NetworkEndpoint.h"

#include <ege/debug/Logger.h>
#include <iostream>

namespace EGE
{

void NetworkEndpoint::disconnect()
{
    if(!m_connected)
        return;
    m_connected = false;

    if(m_socket)
    {
        err(LogLevel::Info) << "001A EGE/network: Disconnecting network endpoint: " << m_socket->getLocalPort();
        m_socket->disconnect();
    }
}

}
