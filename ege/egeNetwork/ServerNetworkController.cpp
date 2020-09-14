/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ServerNetworkController.h"

#include "EGEPacket.h"
#include "EGEServer.h"

namespace EGE
{

void ServerNetworkController::sendRequest(const ControlObject& data)
{
    m_server->sendToAll(EGEPacket::generateSSceneObjectControl(getObject(), data));
}

}
