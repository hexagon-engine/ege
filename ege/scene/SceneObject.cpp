/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject.h"

#include "Scene.h"

#include <ege/util/ObjectString.h>

namespace EGE
{

SceneObject::~SceneObject()
{
    log(LogLevel::Debug) << "SceneObject::~SceneObject() " << this;
}

void SceneObject::onUpdate(long long tickCounter)
{
    (void) tickCounter;
    EventLoop::onUpdate();
}

std::shared_ptr<ObjectMap> SceneObject::serialize() const
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    data->addString("name", m_name);
    data->addObject("m", serializeMain());
    data->addObject("x", serializeExtended());
    return data;
}

bool SceneObject::deserialize(std::shared_ptr<ObjectMap> object)
{
    ASSERT(object);
    auto _name = object->getObject("name");
    auto _m = object->getObject("m");
    auto _x = object->getObject("x");

    m_name = _name.as<EGE::String>().valueOr(m_name);

    bool s = true;

    if(_m.is<ObjectMap::ValueType>())
        s &= deserializeMain(_m.to<ObjectMap>().value());
    else
    {
        err(LogLevel::Verbose) << "No main data key in SceneObject data!";
        return false;
    }

    if(_x.is<ObjectMap::ValueType>())
        s &= deserializeExtended(_x.to<ObjectMap>().value());
    else
        err(LogLevel::Verbose) << "No extended data key in SceneObject data!";
        // They are not required

    m_changedSinceLoad = false;
    return s;
}

std::shared_ptr<ObjectMap> SceneObject::serializeMain() const
{
    auto data = make<ObjectMap>();
    if(m_parent)
        data->addInt("parent", m_parent->getObjectId());
    return data;
}

bool SceneObject::deserializeMain(std::shared_ptr<ObjectMap> data)
{
    auto parentId = data->getObject("parent").as<MaxUint>();
    if(parentId.hasValue())
    {
        setParent(getOwner().getObject(parentId.value()).get());
    }
    return true;
}

std::shared_ptr<ObjectMap> SceneObject::serializeExtended() const
{
    return nullptr;
}

bool SceneObject::deserializeExtended(std::shared_ptr<ObjectMap>)
{
    // nothing
    return true;
}

void SceneObject::setParent(SceneObject* object)
{
    if(m_parent)
        m_parent->m_children.erase(this);

    m_parent = object;
    if(!object)
        return;

    m_parent->m_children.insert(this);
}

}
