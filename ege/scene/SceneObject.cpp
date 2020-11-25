/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject.h"

#include "Scene.h"

#include <ege/util/ObjectString.h>

namespace EGE
{

void SceneObject::onUpdate(long long tickCounter)
{
    (void) tickCounter;
    EventLoop::onUpdate();
}

std::shared_ptr<ObjectMap> SceneObject::serialize()
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    data->addObject("name", make<ObjectString>(m_name));
    data->addObject("m", serializeMain());
    data->addObject("x", serializeExtended());
    return data;
}

bool SceneObject::deserialize(std::shared_ptr<ObjectMap> object)
{
    ASSERT(object);
    auto name = object->getObject("name");
    auto m = object->getObject("m");
    auto x = object->getObject("x");

    if(!name.expired() && name.lock()->isString())
        m_name = name.lock()->asString();

    bool s = true;

    if(!m.expired() && m.lock()->isMap())
        s &= deserializeMain(std::dynamic_pointer_cast<ObjectMap>(std::shared_ptr<Object>(m)));
    else
        return false;

    if(!x.expired() && x.lock()->isMap())
        s &= deserializeExtended(std::dynamic_pointer_cast<ObjectMap>(std::shared_ptr<Object>(x)));
    else
        return false;

    return s;
}

std::shared_ptr<ObjectMap> SceneObject::serializeMain()
{
    return nullptr;
}

bool SceneObject::deserializeMain(std::shared_ptr<ObjectMap>)
{
    // nothing
    return true;
}

std::shared_ptr<ObjectMap> SceneObject::serializeExtended()
{
    return nullptr;
}

bool SceneObject::deserializeExtended(std::shared_ptr<ObjectMap>)
{
    // nothing
    return true;
}

}
