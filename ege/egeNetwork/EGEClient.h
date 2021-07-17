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

#include "ClientNetworkController.h"
#include "EGEGame.h"
#include "EGEPacket.h"

#include <ege/network/Client.h>
#include <ege/network/Packet.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <ege/network/SFMLPacket.h>
#include <ege/util/ObjectMap.h>
#include <memory>
#include <set>

#define EGECLIENT_DEBUG 0

namespace EGE
{

class ExitEvent : public Event
{
public:
    EGE_EVENT("ExitEvent");

    ExitEvent(int rv)
    : returnValue(rv) {}

    int returnValue;
};

class EGEClient : public Client, public SFMLNetworkImpl, public EventLoop, public EGEGame
{
public:
    EGEClient(sf::IpAddress addr, unsigned short port)
    : Client()
    , m_ip(addr)
    , m_port(port) {}

    virtual ~EGEClient();

    virtual SharedPtr<ObjectMap> getLoginData(SharedPtr<ObjectMap>) { return nullptr; }
    virtual void onData(SharedPtr<ObjectMap>) {}

    virtual EventResult onReceive(SharedPtr<Packet> packet);
    EventResult createSceneObjectFromData(SharedPtr<ObjectMap> object, UidType id, String typeId);
    EventResult updateSceneObjectFromData(SharedPtr<ObjectMap> object, UidType id);

    virtual void setScene(SharedPtr<Scene> scene);

    virtual EventResult onLoad();
    virtual void onTick(TickCount);

    virtual void onExit(int) {}
    virtual EventResult onFinish(int exitCode);

    virtual void onDisconnect(String) {}

    virtual bool send(SharedPtr<Packet> packet)
    {
        return sendTo(this, packet);
    }

    bool sendWithUID(SharedPtr<EGEPacket> packet);

    virtual SharedPtr<Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual SharedPtr<SFMLPacket> makePacket(sf::Packet& packet);
    virtual void disconnect();

    SharedPtr<ClientNetworkController> getDefaultController() { return m_defaultController; }
    SharedPtr<ClientNetworkController> getController(UidType objectId);
    SharedPtr<SceneObject> getDefaultControlledObject();
    SharedPtr<SceneObject> getControlledObject(UidType objectId);

    virtual SharedPtr<ClientNetworkController> makeController(SceneObject&) = 0;
    void control(SceneObject* object, const ControlPacket& data);
    void requestControl(SceneObject* object, const ControlPacket& data);

    void requestObject(UidType id);

    void addAdditionalController(UidType id) { m_additionalControllers.insert(id); }
    void removeAdditionalController(UidType id) { m_additionalControllers.erase(id); }
    bool hasAdditionalController(UidType id) { return m_additionalControllers.count(id); }

private:
    Map<UidType, EGEPacket::Type> m_uidMap;
    SharedPtr<AsyncTask> m_clientTask;
    SharedPtr<ClientNetworkController> m_defaultController;
    Map<UidType, SharedPtr<ClientNetworkController>> m_controllersForObjects;
    std::set<UidType> m_requestedObjects;
    Set<UidType> m_additionalControllers;
    sf::IpAddress m_ip;
    unsigned short m_port;
};

}
