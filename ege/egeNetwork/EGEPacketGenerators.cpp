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

#include "EGEPacket.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectString.h>

namespace EGE
{

std::shared_ptr<EGEPacket> EGEPacket::generate_Data(std::shared_ptr<ObjectMap> data)
{
    ASSERT(data);
    EGEPacket::appendUID(data);
    return make<EGEPacket>(EGEPacket::Type::_Data, data);
}

std::shared_ptr<EGEPacket> EGEPacket::generate_Ping()
{
    return make<EGEPacket>(EGEPacket::Type::_Ping, nullptr);
}

std::shared_ptr<EGEPacket> EGEPacket::generate_Pong()
{
    return make<EGEPacket>(EGEPacket::Type::_Pong, nullptr);
}

std::shared_ptr<EGEPacket> EGEPacket::generate_ProtocolVersion(int value)
{
    std::shared_ptr<ObjectMap> object = make<ObjectMap>();
    object->addObject("value", make<ObjectInt>(value));
    return make<EGEPacket>(EGEPacket::Type::_ProtocolVersion, object);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSResult(long long uid, std::shared_ptr<ObjectMap> userData)
{
    userData->addObject("uid", make<ObjectInt>(uid));
    return make<EGEPacket>(EGEPacket::Type::SResult, userData);
}

std::shared_ptr<EGEPacket> EGEPacket::generateCLogin(std::shared_ptr<ObjectMap> userData) //SResult
{
    if(userData)
        EGEPacket::appendUID(userData);
    else
    {
        userData = make<ObjectMap>();
        userData->addObject("uid", make<ObjectInt>(generateUID()));
    }
    return make<EGEPacket>(EGEPacket::Type::CLogin, userData);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSLoginRequest(std::shared_ptr<ObjectMap> userData)
{
    return make<EGEPacket>(EGEPacket::Type::SLoginRequest, userData);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSDisconnectReason(std::string message)
{
    std::shared_ptr<ObjectMap> object = make<ObjectMap>();
    object->addObject("message", make<ObjectString>(message));
    return make<EGEPacket>(EGEPacket::Type::SDisconnectReason, object);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneObjectCreation(std::shared_ptr<SceneObject> object, std::string typeId)
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    data->addObject("object", object->serialize());
    data->addObject("id", make<ObjectInt>(object->getObjectId()));
    data->addObject("typeId", make<ObjectString>(typeId));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectCreation, data);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneObjectUpdate_Main(std::shared_ptr<SceneObject> object)
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    std::shared_ptr<ObjectMap> objectData = make<ObjectMap>();
    objectData->addObject("m", object->serializeMain());
    data->addObject("object", objectData);
    data->addObject("id", make<ObjectInt>(object->getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectUpdate, data);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneObjectUpdate_Extended(std::shared_ptr<SceneObject> object)
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    std::shared_ptr<ObjectMap> objectData = make<ObjectMap>();
    objectData->addObject("x", object->serializeExtended());
    data->addObject("object", objectData);
    data->addObject("id", make<ObjectInt>(object->getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectUpdate, data);
}
std::shared_ptr<EGEPacket> EGEPacket::generateSSceneObjectDeletion(long long id)
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    data->addObject("id", make<ObjectInt>(id));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectDeletion, data);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneCreation(std::shared_ptr<ObjectMap>)
{
    ASSERT(false); //TODO
    return nullptr;
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneDeletion(std::shared_ptr<ObjectMap>)
{
    ASSERT(false); //TODO
    return nullptr;
}

std::shared_ptr<EGEPacket> EGEPacket::generateCSceneObjectControl(std::shared_ptr<SceneObject> object, const ControlObject& data) //SResult
{
    std::shared_ptr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(object->getObjectId()));
    std::shared_ptr<ObjectMap> args_data = make<ObjectMap>();
    args_data->addObject("type", make<ObjectString>(data.getType()));
    args_data->addObject("args", data.getArgs() ? data.getArgs() : make<ObjectMap>());
    args->addObject("data", args_data);
    return make<EGEPacket>(EGEPacket::Type::CSceneObjectControl, args);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSDefaultControllerId(std::shared_ptr<SceneObject> object)
{
    std::shared_ptr<ObjectMap> args = make<ObjectMap>();
    if(!object)
        args->addObject("id", make<ObjectInt>(0));
    else
        args->addObject("id", make<ObjectInt>(object->getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SDefaultControllerId, args);
}

std::shared_ptr<EGEPacket> EGEPacket::generateCSceneObjectRequest(long long id)
{
    std::shared_ptr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(id));
    return make<EGEPacket>(EGEPacket::Type::CSceneObjectRequest, args);
}

std::shared_ptr<EGEPacket> EGEPacket::generateSSceneObjectControl(std::shared_ptr<SceneObject> object, const ControlObject& data)
{
    std::shared_ptr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(object->getObjectId()));
    std::shared_ptr<ObjectMap> args_data = make<ObjectMap>();
    args_data->addObject("type", make<ObjectString>(data.getType()));
    args_data->addObject("args", data.getArgs() ? data.getArgs() : make<ObjectMap>());
    args->addObject("data", args_data);
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectControl, args);
}

std::shared_ptr<EGEPacket> EGEPacket::generate_Version(int value, std::string str)
{
    std::shared_ptr<ObjectMap> args = make<ObjectMap>();
    args->addInt("value", value);
    args->addString("string", str);
    return make<EGEPacket>(EGEPacket::Type::_Version, args);
}

}
