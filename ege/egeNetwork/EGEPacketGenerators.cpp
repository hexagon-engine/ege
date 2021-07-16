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
#include <ege/util/ObjectBoolean.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectString.h>

namespace EGE
{

SharedPtr<EGEPacket> EGEPacket::generate_Data(SharedPtr<ObjectMap> data)
{
    ASSERT(data);
    EGEPacket::appendUID(data);
    return make<EGEPacket>(EGEPacket::Type::_Data, data);
}

SharedPtr<EGEPacket> EGEPacket::generate_Ping()
{
    return make<EGEPacket>(EGEPacket::Type::_Ping, nullptr);
}

SharedPtr<EGEPacket> EGEPacket::generate_Pong()
{
    return make<EGEPacket>(EGEPacket::Type::_Pong, nullptr);
}

SharedPtr<EGEPacket> EGEPacket::generate_ProtocolVersion(int value)
{
    SharedPtr<ObjectMap> object = make<ObjectMap>();
    object->addObject("value", make<ObjectInt>(value));
    return make<EGEPacket>(EGEPacket::Type::_ProtocolVersion, object);
}

SharedPtr<EGEPacket> EGEPacket::generateSResult(UidType uid, SharedPtr<ObjectMap> userData)
{
    userData->addObject("uid", make<ObjectInt>(uid));
    return make<EGEPacket>(EGEPacket::Type::SResult, userData);
}

SharedPtr<EGEPacket> EGEPacket::generateCLogin(SharedPtr<ObjectMap> userData) //SResult
{
    // FIXME: Create ObjectMap if required
    if(userData)
        EGEPacket::appendUID(userData);
    else
    {
        userData = make<ObjectMap>();
        userData->addObject("uid", make<ObjectInt>(generateUID()));
    }
    return make<EGEPacket>(EGEPacket::Type::CLogin, userData);
}

SharedPtr<EGEPacket> EGEPacket::generateSLoginRequest(SharedPtr<ObjectMap> userData)
{
    return make<EGEPacket>(EGEPacket::Type::SLoginRequest, userData);
}

SharedPtr<EGEPacket> EGEPacket::generateSDisconnectReason(std::string message)
{
    SharedPtr<ObjectMap> object = make<ObjectMap>();
    object->addObject("message", make<ObjectString>(message));
    return make<EGEPacket>(EGEPacket::Type::SDisconnectReason, object);
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneObjectCreation(SceneObject& object, std::string typeId)
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();
    data->addObject("object", object.serialize());
    data->addObject("id", make<ObjectInt>(object.getObjectId()));
    data->addObject("typeId", make<ObjectString>(typeId));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectCreation, data);
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneObjectUpdate_Main(SceneObject& object)
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();
    SharedPtr<ObjectMap> objectData = make<ObjectMap>();
    objectData->addObject("m", object.serializeMain());
    data->addObject("object", objectData);
    data->addObject("id", make<ObjectInt>(object.getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectUpdate, data);
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneObjectUpdate_Extended(SceneObject& object)
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();
    SharedPtr<ObjectMap> objectData = make<ObjectMap>();
    objectData->addObject("x", object.serializeExtended());
    data->addObject("object", objectData);
    data->addObject("id", make<ObjectInt>(object.getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectUpdate, data);
}
SharedPtr<EGEPacket> EGEPacket::generateSSceneObjectDeletion(UidType id)
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();
    data->addObject("id", make<ObjectInt>(id));
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectDeletion, data);
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneCreation(SharedPtr<ObjectMap>)
{
    ASSERT(false); //TODO
    return nullptr;
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneDeletion(SharedPtr<ObjectMap>)
{
    ASSERT(false); //TODO
    return nullptr;
}

SharedPtr<EGEPacket> EGEPacket::generateCSceneObjectControl(SceneObject& object, const ControlPacket& data) //SResult
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(object.getObjectId()));
    SharedPtr<ObjectMap> args_data = make<ObjectMap>();
    args_data->addObject("type", make<ObjectString>(data.getType()));
    args_data->addObject("args", data.getArgs() ? data.getArgs() : make<ObjectMap>());
    args->addObject("data", args_data);
    return make<EGEPacket>(EGEPacket::Type::CSceneObjectControl, args);
}

SharedPtr<EGEPacket> EGEPacket::generateSDefaultControllerId(SceneObject* object)
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    if(!object)
        args->addObject("id", make<ObjectInt>(0));
    else
        args->addObject("id", make<ObjectInt>(object->getObjectId()));
    return make<EGEPacket>(EGEPacket::Type::SDefaultControllerId, args);
}

SharedPtr<EGEPacket> EGEPacket::generateCSceneObjectRequest(UidType id)
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(id));
    return make<EGEPacket>(EGEPacket::Type::CSceneObjectRequest, args);
}

SharedPtr<EGEPacket> EGEPacket::generateSSceneObjectControl(SceneObject& object, const ControlPacket& data)
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    args->addObject("id", make<ObjectInt>(object.getObjectId()));
    SharedPtr<ObjectMap> args_data = make<ObjectMap>();
    args_data->addObject("type", make<ObjectString>(data.getType()));
    args_data->addObject("args", data.getArgs() ? data.getArgs() : make<ObjectMap>());
    args->addObject("data", args_data);
    return make<EGEPacket>(EGEPacket::Type::SSceneObjectControl, args);
}

SharedPtr<EGEPacket> EGEPacket::generate_Version(int value, std::string str)
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    args->addInt("value", value);
    args->addString("string", str);
    return make<EGEPacket>(EGEPacket::Type::_Version, args);
}

SharedPtr<EGEPacket> EGEPacket::generateSAdditionalControllerId(SceneObject& object, bool remove)
{
    SharedPtr<ObjectMap> args = make<ObjectMap>();
    args->addInt("id", object.getObjectId());
    args->addObject("remove", make<ObjectBoolean>(remove));
    return make<EGEPacket>(EGEPacket::Type::SAdditionalControllerId, args);
}

}
