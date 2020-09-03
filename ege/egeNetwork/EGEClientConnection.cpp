/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEClientConnection.h"

#include "EGEPacket.h"

namespace EGE
{

std::shared_ptr<SFMLPacket> EGEClientConnection::makePacket(sf::Packet& packet)
{
    return std::make_shared<EGEPacket>(packet);
}

void EGEClientConnection::setLastRecvTime(time_t t)
{
    m_lastRecv = t;
}

}
