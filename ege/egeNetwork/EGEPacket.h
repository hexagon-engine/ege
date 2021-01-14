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

#pragma once

#include <ege/controller/ControlObject.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/network/SFMLPacket.h>
#include <ege/scene/SceneObject.h>
#include <ege/util/ObjectMap.h>
#include <memory>

// Protocol version used by this implementation.
// It's reported by _ProtocolVersion packet.
#define EGE_PROTOCOL_VERSION 0

#define EGEPACKET_DEBUG 0

namespace EGE
{

class EGEPacket : public SFMLPacket
{
public:
    enum class Type
    {
        // Prefixes:
        // S-server
        // C-client
        // _-both
        _Data = 0x00,
        _Ping = 0x01,
        _Pong = 0x02,
        _ProtocolVersion = 0x03, //current: 0x0 (unknown)
        SResult = 0x04,
        CLogin = 0x05,
        SLoginRequest = 0x06,
        SDisconnectReason = 0x07,
        SSceneObjectCreation = 0x08,
        SSceneObjectUpdate = 0x09,
        SSceneObjectDeletion = 0x0a,
        SSceneCreation = 0x0b,
        SSceneDeletion = 0x0c,
        CSceneObjectControl = 0x0d,
        SDefaultControllerId = 0x0e,
        CSceneObjectRequest = 0x0f,
        SSceneObjectControl = 0x10,
        _Version = 0x11 // defined for EGEGame.
    };

    static std::string typeString(Type type);

    // receiver
    EGEPacket(sf::Packet& data)
    {
        fromSFMLPacket(data);
    }

    // sender
    EGEPacket(Type type, std::shared_ptr<ObjectMap> args)
    : m_type(type)
    , m_args(args)
    {}

    bool fromSFMLPacket(sf::Packet& packet);
    virtual sf::Packet toSFMLPacket();

    Type getType()
    {
        return m_type;
    }

    std::shared_ptr<ObjectMap> getArgs()
    {
        return m_args;
    }

    static long long generateUID();
    static void appendUID(std::shared_ptr<ObjectMap> packetArgs);

    static std::shared_ptr<EGEPacket> generate_Data(std::shared_ptr<ObjectMap> data);
    static std::shared_ptr<EGEPacket> generate_Ping();
    static std::shared_ptr<EGEPacket> generate_Pong();
    static std::shared_ptr<EGEPacket> generate_ProtocolVersion(int value);
    static std::shared_ptr<EGEPacket> generateSResult(long long uid, std::shared_ptr<ObjectMap> userData = nullptr);
    static std::shared_ptr<EGEPacket> generateCLogin(std::shared_ptr<ObjectMap> userData = nullptr); //SResult
    static std::shared_ptr<EGEPacket> generateSLoginRequest(std::shared_ptr<ObjectMap> userData = nullptr);
    static std::shared_ptr<EGEPacket> generateSDisconnectReason(std::string message);
    static std::shared_ptr<EGEPacket> generateSSceneObjectCreation(std::shared_ptr<SceneObject> object, std::string typeId);
    static std::shared_ptr<EGEPacket> generateSSceneObjectUpdate_Main(std::shared_ptr<SceneObject> object);
    static std::shared_ptr<EGEPacket> generateSSceneObjectUpdate_Extended(std::shared_ptr<SceneObject> object);
    static std::shared_ptr<EGEPacket> generateSSceneObjectDeletion(long long id);
    static std::shared_ptr<EGEPacket> generateSSceneCreation(std::shared_ptr<ObjectMap> userData = nullptr);
    static std::shared_ptr<EGEPacket> generateSSceneDeletion(std::shared_ptr<ObjectMap> userData = nullptr);
    static std::shared_ptr<EGEPacket> generateCSceneObjectControl(std::shared_ptr<SceneObject> object, const ControlObject& data); //SResult
    static std::shared_ptr<EGEPacket> generateSDefaultControllerId(std::shared_ptr<SceneObject> object);
    static std::shared_ptr<EGEPacket> generateCSceneObjectRequest(long long id);
    static std::shared_ptr<EGEPacket> generateSSceneObjectControl(std::shared_ptr<SceneObject> object, const ControlObject& data);
    static std::shared_ptr<EGEPacket> generate_Version(int value, std::string str);

private:
    Type m_type;
    std::shared_ptr<ObjectMap> m_args;
};

}
