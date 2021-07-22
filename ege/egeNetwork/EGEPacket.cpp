/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include "EGEPacket.h"

#include "EGEPacketConverter.h"

#include <cstdlib>
#include <ctime>
#include <ege/util/Converter.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/Random.h>
#include <ege/util/system.h>
#include <SFML/Network.hpp>

namespace EGE
{

std::string EGEPacket::typeString(Type type)
{
    switch(type)
    {
        case EGEPacket::Type::_Data: return "_Data";
        case EGEPacket::Type::_Ping: return "_Ping";
        case EGEPacket::Type::_Pong: return "_Pong";
        case EGEPacket::Type::_ProtocolVersion: return "_ProtocolVersion";
        case EGEPacket::Type::SResult: return "SResult";
        case EGEPacket::Type::CLogin: return "CLogin";
        case EGEPacket::Type::SLoginRequest: return "SLoginRequest";
        case EGEPacket::Type::SDisconnectReason: return "SDisconnectReason";
        case EGEPacket::Type::SSceneObjectCreation: return "SSceneObjectCreation";
        case EGEPacket::Type::SSceneObjectUpdate: return "SSceneObjectUpdate";
        case EGEPacket::Type::SSceneObjectDeletion: return "SSceneObjectDeletion";
        case EGEPacket::Type::SSceneCreation: return "SSceneCreation";
        case EGEPacket::Type::SSceneDeletion: return "SSceneDeletion";
        case EGEPacket::Type::CSceneObjectControl: return "CSceneObjectControl";
        case EGEPacket::Type::SDefaultControllerId: return "SDefaultControllerId";
        case EGEPacket::Type::CSceneObjectRequest: return "CSceneObjectRequest";
        case EGEPacket::Type::SSceneObjectControl: return "SSceneObjectControl";
        case EGEPacket::Type::_Version: return "_Version";
        case EGEPacket::Type::SAdditionalControllerId: return "SAdditionalControllerId";
        default: return "<unknown>";
    }
}

bool EGEPacket::deserializeFromSFML(sf::Packet& packet)
{
    unsigned int type;
    packet >> type;
    m_type = static_cast<Type>(type);
    SharedPtr<Object> args = make<ObjectMap>();
    if(!(packet >> objectIn(args, EGEPacketConverter())))
        return false;
    m_args = std::dynamic_pointer_cast<ObjectMap>(args);
    return true;
}

sf::Packet EGEPacket::serializeToSFML() const
{
    sf::Packet packet;
    packet << (unsigned int)m_type;

    if(m_args)
        EGEPacketConverter().out(packet, *m_args);

    return packet;
}

long long EGEPacket::generateUID()
{
    static EGE::Random random(EGE::System::unixTime());
    static EGE::UidType lastUID = 0;
    lastUID += random.nextIntRanged(1, 4);
    return lastUID;
}

void EGEPacket::appendUID(SharedPtr<ObjectMap> packetArgs)
{
    packetArgs->addObject("uid", make<ObjectInt>(generateUID()));
}

}
