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

    // Called when this controller is set as default on Client.
    virtual void onSetDefault() {}

private:
    EGEClient* m_client;
};

}
