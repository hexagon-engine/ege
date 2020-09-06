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

void EGEClientConnection::setLastRecvTime(Time t)
{
    m_lastRecv = t;
}

}
