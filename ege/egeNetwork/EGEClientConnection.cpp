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

#include "EGEClientConnection.h"
#include "EGEServer.h"

#include "EGEPacket.h"

namespace EGE
{

EGEClientConnection::EGEClientConnection(EGEServer& server)
: TcpClientConnection(server)
, m_lastRecv(server.time(Time::Unit::Seconds))
, m_createTime(server.time(Time::Unit::Seconds)) {}

void EGEClientConnection::disconnectWithReason(String reason)
{
    ege_log.info() << "EGEClientConnection: Disconnecting " << toString() << " with reason: " << reason;
    send(EGEPacket::generateSDisconnectReason(reason));
    disconnect();
}

bool EGEClientConnection::canControl(SceneObject const& object) const
{
    return getControlledSceneObject() == &object || hasAdditionalController(object);
}

void EGEClientConnection::setControlledSceneObject(SceneObject const* object)
{
    m_defaultControlledObject = object;
    ege_log.info() << "EGEClientConnection: Set default controller to " << (object ? object->getName() : "<null>") << " for " << toString();
    send(EGEPacket::generateSDefaultControllerId(object));
}

void EGEClientConnection::addAdditionalController(SceneObject const& object)
{
    m_additionalControlledObjects.insert(&object);
    ege_log.info() << "EGEClientConnection: Add additional controller " << object.getName() << " for " << toString();
    send(EGEPacket::generateSAdditionalControllerId(object, false));
}

void EGEClientConnection::removeAdditionalController(SceneObject const& object)
{
    m_additionalControlledObjects.erase(&object);
    ege_log.info() << "EGEClientConnection: Remove additional controller " << object.getName() << " for " << toString();
    send(EGEPacket::generateSAdditionalControllerId(object, true));
}

}
