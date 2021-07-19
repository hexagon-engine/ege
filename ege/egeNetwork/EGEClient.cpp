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

#include "EGEClient.h"

#include "EGEPacket.h"

#include <ege/core/AsyncTask.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

void EGEClient::onReceive(EGEPacket const& packet)
{
    auto type = packet.getType();
    auto args = packet.getArgs();

    ege_log.info() << EGEPacket::typeString(type);

    switch(type)
    {
        case EGEPacket::Type::SResult:
        {
        } break;
        case EGEPacket::Type::SLoginRequest:
        {
            send(EGEPacket::generateCLogin(m_loginData));
        } break;
        case EGEPacket::Type::SDisconnectReason:
        {
            auto message = args->get("message").asString().valueOr("???");
            ege_log.notice() << "EGEClient: Disconnected from server: " << message;
            disconnect();
        } break;
        case EGEPacket::Type::SSceneObjectCreation:
        {
            auto typeId = args->get("typeId").asString();
            if(!typeId.hasValue())
            {
                disconnect();
                return;
            }
            auto id = args->get("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }
            auto object = args->get("object").to<ObjectMap>();
            if(!object.hasValue())
            {
                disconnect();
                return;
            }
            auto scene = getScene();
            if(!scene)
            {
                disconnect();
                return;
            }
            auto sceneObject = scene->createObject(typeId.value(), object.value());
            sceneObject->setObjectId(id.value());
            scene->addObject(sceneObject);
        } break;
        case EGEPacket::Type::SSceneObjectUpdate:
        {
            auto object = args->get("object").to<ObjectMap>();
            if(!object.hasValue())
            {
                disconnect();
                return;
            }
            auto id = args->get("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }
            auto scene = getScene();
            if(!scene)
            {
                disconnect();
                return;
            }
            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject)
            {
                // TODO: Request object
                disconnect();
                return;
            }
            if(!sceneObject->deserialize(object.value()))
            {
                disconnect();
                return;
            }
        } break;
        case EGEPacket::Type::SSceneObjectDeletion:
        {
            auto id = args->get("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }
            auto scene = getScene();
            if(!scene)
            {
                disconnect();
                return;
            }
            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject)
            {
                // TODO: Request object
                disconnect();
                return;
            }
            if(m_controlledObject == sceneObject.get())
                setDefaultController(nullptr);
            sceneObject->setDead();
        } break;
        case EGEPacket::Type::SDefaultControllerId:
        {
            auto id = args->get("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }
            auto scene = getScene();
            if(!scene)
            {
                disconnect();
                return;
            }
            setDefaultController(scene->getObject(id.value()).get());
        } break;
    case EGEPacket::Type::SSceneObjectControl:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            auto data = args->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
            {
                disconnect();
                return;
            }

            auto data_name = data.value()->getObject("type").asString();
            if(!data_name.hasValue())
            {
                disconnect();
                return;
            }

            auto data_args = data.value()->getObject("args").to<ObjectMap>();
            if(!data_args.hasValue())
            {
                disconnect();
                return;
            }

            auto scene = getScene();
            if(scene) // cannot control object when no scene is created!
            {
                disconnect();
                return;
            }

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject)
            {
                disconnect();
                return;
            }

            runController(*sceneObject, ControlPacket(data_name.value(), data_args.value()));
        }
        break;
        case EGEPacket::Type::_Ping:
        {
            send(EGEPacket::generate_Pong());
        } break;
        case EGEPacket::Type::_Pong:
        {
            // TODO: Keep-alive
        } break;
        case EGEPacket::Type::_ProtocolVersion:
        {
            auto value = args->get("value").asInt().valueOr(-1);
            if(value != EGE_PROTOCOL_VERSION)
            {
                ege_log.error() << "Protocol version check failed: " << value << " != " << EGE_PROTOCOL_VERSION;
                disconnect();
            }
            if(!m_protocolCheckSuccess)
            {
                send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));
                m_protocolCheckSuccess = true;
            }
        } break;
        case EGEPacket::Type::_Version:
        {
            auto value = args->get("value").asInt().valueOr(-1);
            if(value != getVersion())
            {
                ege_log.error() << "Agent version check failed: " << value << " != " << getVersion();
                disconnect();
            }
            auto string = args->get("string").asString().valueOr("");
            if(string != getVersionString())
            {
                ege_log.error() << "Agent string check failed: " << string << " != " << getVersionString();
                disconnect();
            }
            if(!m_agentCheckSuccess)
            {
                send(EGEPacket::generate_Version(getVersion(), getVersionString()));
                m_agentCheckSuccess = true;
            }
        } break;
        default:
        {
            ege_log.warning() << "EGEClient: Unimplemented EGEPacket type: " << EGEPacket::typeString(type);
        } break;
    }
}

void EGEClient::control(SceneObject const& object, std::string action, std::shared_ptr<ObjectMap> data)
{
    send(EGEPacket::generateCSceneObjectControl(object, ControlPacket(action, data)));
}

void EGEClient::control(String action, SharedPtr<ObjectMap> data)
{
    if(!m_controlledObject)
    {
        ege_log.warning() << "EGEClient: Control '" + action + "' requested when no default controller!";
        return;
    }
    send(EGEPacket::generateCSceneObjectControl(*m_controlledObject, ControlPacket(action, data)));
}

}
