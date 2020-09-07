/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <ege/network/SFMLPacket.h>
#include <ege/scene/SceneObject.h>
#include <ege/util/ObjectMap.h>
#include <memory>

// Protocol version used by this implementation.
// It's reported by _ProtocolVersion packet.
#define EGE_PROTOCOL_VERSION 0

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
        SSceneObjectControl = 0x0e
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
    static std::shared_ptr<EGEPacket> generateCSceneObjectControl(long long id, std::shared_ptr<ObjectMap> userData = nullptr); //SResult
    static std::shared_ptr<EGEPacket> generateSSceneObjectControl(long long id, std::shared_ptr<ObjectMap> userData = nullptr);

private:
    Type m_type;
    std::shared_ptr<ObjectMap> m_args;
};

}