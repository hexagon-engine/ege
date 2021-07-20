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

#include <ege/egeNetwork/ControlPacket.h>
#include <ege/main/Config.h>
#include <ege/network/SFMLPacket.h>
#include <ege/scene/SceneObject.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <memory>

// Protocol version used by this implementation.
// It's reported by _ProtocolVersion packet.
#define EGE_PROTOCOL_VERSION 0

#define EGEPACKET_DEBUG 1

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
        _Version = 0x11, // defined for EGEGame.
        SAdditionalControllerId = 0x12
    };

    EGEPacket() = default;

    EGEPacket(Type type, SharedPtr<ObjectMap> args)
    : m_type(type), m_args(args) {}

    static std::string typeString(Type type);

    virtual bool deserializeFromSFML(sf::Packet& packet) override;
    virtual sf::Packet serializeToSFML() const override;

    Type getType() const { return m_type; }
    SharedPtr<ObjectMap> getArgs() const { return m_args; }

    static long long generateUID();
    static void appendUID(SharedPtr<ObjectMap> packetArgs);

    static EGEPacket generate_Data(SharedPtr<ObjectMap> data);
    static EGEPacket generate_Ping();
    static EGEPacket generate_Pong();
    static EGEPacket generate_ProtocolVersion(int value);
    static EGEPacket generateSResult(UidType uid, SharedPtr<ObjectMap> userData = nullptr);
    static EGEPacket generateCLogin(SharedPtr<ObjectMap> userData = nullptr); //SResult
    static EGEPacket generateSLoginRequest(SharedPtr<ObjectMap> userData = nullptr);
    static EGEPacket generateSDisconnectReason(std::string message);
    static EGEPacket generateSSceneObjectCreation(SceneObject& object, std::string typeId);
    static EGEPacket generateSSceneObjectUpdate_Main(SceneObject& object);
    static EGEPacket generateSSceneObjectUpdate_Extended(SceneObject& object);
    static EGEPacket generateSSceneObjectDeletion(UidType id);
    static EGEPacket generateSSceneCreation(SharedPtr<ObjectMap> userData = nullptr);
    static EGEPacket generateSSceneDeletion(SharedPtr<ObjectMap> userData = nullptr);
    static EGEPacket generateCSceneObjectControl(SceneObject const& object, const ControlPacket& data); //SResult
    static EGEPacket generateSDefaultControllerId(SceneObject const* object);
    static EGEPacket generateCSceneObjectRequest(UidType id);
    static EGEPacket generateSSceneObjectControl(SceneObject const& object, const ControlPacket& data);
    static EGEPacket generate_Version(int value, std::string str);
    static EGEPacket generateSAdditionalControllerId(SceneObject const& object, bool remove);

private:
    Type m_type;
    SharedPtr<ObjectMap> m_args;
};

}
