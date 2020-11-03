/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/controller/Controller.h>
#include <ege/scene/SceneObject.h>
#include <memory>

namespace EGE
{

class EGEServer;

class ServerNetworkController : public Controller
{
public:
    ServerNetworkController(std::shared_ptr<SceneObject> object, EGEServer* server)
    : Controller(std::static_pointer_cast<Controllable>(object)), m_server(server) { ASSERT(object); ASSERT(server); }

    // send request to CLIENT.
    virtual void sendRequest(const ControlObject& data);

private:
    EGEServer* m_server;
};

}
