/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEPacket.h"

#include "EGEPacketConverter.h"

#include <cstdlib>
#include <ctime>
#include <ege/util/Converter.h>
#include <ege/util/ObjectInt.h>

namespace EGE
{

bool EGEPacket::fromSFMLPacket(sf::Packet& packet)
{
    bool success = true;
    success &= !packet.endOfPacket();
    packet >> (unsigned int&)m_type;
    success &= !packet.endOfPacket();
    m_args = std::make_shared<ObjectMap>();
    return packet >> objectIn(*m_args, EGEPacketConverter());
}

sf::Packet EGEPacket::toSFMLPacket()
{
    sf::Packet packet;
    packet << (unsigned int)m_type;

    if(m_args.get())
        packet << objectOut(*m_args, EGEPacketConverter());

    return packet;
}

long long EGEPacket::generateUID()
{
    static long long lastId = []()->long long {
        srand(time(NULL));
        return 0LL;
    }();

    lastId += rand() % 3 + 1;
    return lastId;
}

void EGEPacket::appendUID(std::shared_ptr<ObjectMap> packetArgs)
{
    packetArgs->addObject("uid", std::make_shared<ObjectInt>(generateUID()));
}

}
