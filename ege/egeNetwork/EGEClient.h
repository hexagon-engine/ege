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

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/gui/GameLoop.h>
#include <ege/network/Client.h>
#include <ege/network/Packet.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <ege/network/SFMLPacket.h>
#include <ege/util/ObjectMap.h>
#include <memory>
#include <set>

#define EGECLIENT_DEBUG 1

namespace EGE
{

class EGEClient : public Client, public SFMLNetworkImpl, public GameLoop, public EGEGame
{
public:
    EGEClient(sf::IpAddress addr, unsigned short port)
    : Client()
    , EGEGame()
    , m_ip(addr)
    , m_port(port) {}

    virtual ~EGEClient();

    virtual std::shared_ptr<ObjectMap> getLoginData(std::shared_ptr<ObjectMap>) { return nullptr; }
    virtual void onData(std::shared_ptr<ObjectMap>) {}

    virtual EventResult onReceive(std::shared_ptr<Packet> packet);
    EventResult createSceneObjectFromData(std::shared_ptr<ObjectMap> object, UidType id, std::string typeId);
    EventResult updateSceneObjectFromData(std::shared_ptr<ObjectMap> object, UidType id);

    virtual void setScene(std::shared_ptr<Scene> scene);

    virtual EventResult onLoad();
    virtual void onTick(TickCount);

    virtual void onExit(int) {}
    virtual EventResult onFinish(int exitCode);

    virtual void onDisconnect(std::string) {}

    virtual bool send(std::shared_ptr<Packet> packet)
    {
        return sendTo(this, packet);
    }

    bool sendWithUID(std::shared_ptr<EGEPacket> packet);

    virtual std::shared_ptr<Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual std::shared_ptr<SFMLPacket> makePacket(sf::Packet& packet);
    virtual void disconnect();

    void setExitHandler(std::function<void(int)> func) { m_exitHandler = func; }

    std::shared_ptr<ClientNetworkController> getDefaultController() { return m_defaultController; }
    std::shared_ptr<ClientNetworkController> getController(UidType objectId);
    std::shared_ptr<SceneObject> getDefaultControlledObject();
    std::shared_ptr<SceneObject> getControlledObject(UidType objectId);

    virtual std::shared_ptr<ClientNetworkController> makeController(std::shared_ptr<SceneObject>) { return nullptr; }
    void control(std::shared_ptr<SceneObject> object, const ControlObject& data);
    void requestControl(std::shared_ptr<SceneObject> object, const ControlObject& data);

    void requestObject(UidType id);

private:
    std::map<UidType, EGEPacket::Type> m_uidMap;
    std::shared_ptr<AsyncTask> m_clientTask;
    std::shared_ptr<ClientNetworkController> m_defaultController;
    std::map<UidType, std::shared_ptr<ClientNetworkController>> m_controllersForObjects;
    std::function<void(int)> m_exitHandler;
    std::set<UidType> m_requestedObjects;
    sf::IpAddress m_ip;
    unsigned short m_port;
};

}
