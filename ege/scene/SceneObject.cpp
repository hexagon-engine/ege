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

void SceneObject::deserialize(std::shared_ptr<ObjectMap> object)
{
    ASSERT(object);
    auto name = object->getObject("name");
    auto m = object->getObject("m");
    auto x = object->getObject("x");

    if(!name.expired() && name.lock()->isString())
        m_name = name.lock()->asString();

    if(!m.expired() && m.lock()->isMap())
        deserializeMain(std::dynamic_pointer_cast<ObjectMap>(std::shared_ptr<Object>(m)));
    if(!x.expired() && x.lock()->isMap())
        deserializeExtended(std::dynamic_pointer_cast<ObjectMap>(std::shared_ptr<Object>(x)));
}

std::shared_ptr<ObjectMap> SceneObject::serializeMain()
{
    return nullptr;
}

void SceneObject::deserializeMain(std::shared_ptr<ObjectMap>)
{
    // nothing
}

std::shared_ptr<ObjectMap> SceneObject::serializeExtended()
{
    return nullptr;
}

void SceneObject::deserializeExtended(std::shared_ptr<ObjectMap>)
{
    // nothing
}

}
