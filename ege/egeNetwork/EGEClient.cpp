/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEClient.h"

#include "EGEPacket.h"

#include <ege/asyncLoop/AsyncTask.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

// START HEX DUMP
struct HexDumpSettings
{
    int width;
};

static void hexDump(const void* data, size_t size, HexDumpSettings settings)
{
    for(size_t s = 0; s < size / settings.width + 1; s++)
    {
        std::cerr << std::hex << std::setfill('0') << std::setw(8) << s * settings.width << "   ";

        // data as HEX DUMP
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                std::cerr << std::hex << std::setfill('0') << std::setw(2) << (int)_data << " ";
            }
            else
            {
                std::cerr << "   ";
            }
        }

        std::cerr << "  ";

        // and as CHARACTERS
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                if(_data < 32 || _data >= 127)
                    _data = '.';
                std::cerr << _data << " ";
            }
        }
        std::cout << std::endl;
    }
}
// END HEX DUMP

bool EGEClient::sendWithUID(std::shared_ptr<EGEPacket> packet)
{
    // FIXME: some assertion?
    long long uid = packet->getArgs()->getObject("uid").lock()->asInt();
    m_uidMap[uid] = packet->getType();
    return send(packet);
}

EventResult EGEClient::onReceive(std::shared_ptr<Packet> packet)
{
    EGEPacket* egePacket = (EGEPacket*)packet.get();

    if constexpr(EGECLIENT_DEBUG)
    {
        sf::Packet sfPacket = egePacket->toSFMLPacket();
        std::cerr << "Client: Hex dump: " << std::endl;
        hexDump(sfPacket.getData(), sfPacket.getDataSize(), HexDumpSettings{8});
    }

    if constexpr(EGECLIENT_DEBUG)
    {
        std::cerr << "Client: Received EGEPacket, type=" << EGEPacket::typeString(egePacket->getType()) << std::endl;
        std::cerr << "args=" << egePacket->getArgs()->toString() << std::endl;
    }

    switch(egePacket->getType())
    {
    case EGEPacket::Type::_Ping:
        send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            int value = egePacket->getArgs()->getObject("value").lock()->asInt();
            if(value != EGE_PROTOCOL_VERSION)
            {
                std::cerr << "0020 EGE/egeNetwork: Server PROTOCOL_VERSION doesn't match client! (required "
                    << EGE_PROTOCOL_VERSION << ", got " << value << ")" << std::endl;
                return EventResult::Failure;
            }
            send(EGEPacket::generate_Pong());
        }
        break;
    case EGEPacket::Type::SLoginRequest:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            sendWithUID(EGEPacket::generateCLogin(getLoginData(args)));
        }
        break;
    default:
        DBG(1, "EGEClient::onReceive: not implemented packet handler");
        return EventResult::Failure;
    }

    return EventResult::Success;
}

EventResult EGEClient::onLoad()
{
    // Run client thread
    auto clientNetworkWorker = [this]()->int {
        std::cerr << "001E EGE/egeNetwork: Starting client" << std::endl;
        if(!connect(m_ip, m_port))
            return 1;

        send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));

        while(isRunning())
            update();

        return 0;
    };
    auto clientNetworkCallback = [this](AsyncTask::State state) {
        std::cerr << "001F EGE/egeNetwork: Closing client" << std::endl;
        exit(state.returnCode);
    };

    std::shared_ptr<AsyncTask> task = std::make_shared<AsyncTask>(clientNetworkWorker, clientNetworkCallback);
    addAsyncTask(task, "EGEServer network task");

    return EventResult::Success;
}

void EGEClient::onExit(int exitCode)
{
    (void) exitCode;
    disconnect();
}

void EGEClient::onTick(long long tickCount)
{

}

std::shared_ptr<SFMLPacket> EGEClient::makePacket(sf::Packet& packet)
{
    return std::make_shared<EGEPacket>(packet);
}

}
