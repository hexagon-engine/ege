/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ClientNetworkController.h"

#include <ege/egeNetwork/EGEClient.h>

namespace EGE
{

void ClientNetworkController::sendRequest(const ControlObject& data)
{
    m_client->send(EGEPacket::generateCSceneObjectControl(std::static_pointer_cast<SceneObject>(getObject()), data));
}

}
