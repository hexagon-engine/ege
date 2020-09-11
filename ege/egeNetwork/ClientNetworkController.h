/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/controller/Controller.h>

namespace EGE
{

class EGEClient;

class ClientNetworkController : public Controller
{
public:
    ClientNetworkController(std::shared_ptr<SceneObject> object, EGEClient* client)
    : Controller(object), m_client(client) { ASSERT(object); ASSERT(client); }

    // send request to SERVER.
    virtual void sendRequest(const ControlObject& data);

private:
    EGEClient* m_client;
};

}
