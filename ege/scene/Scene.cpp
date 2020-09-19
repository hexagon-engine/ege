/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene.h"

#include <algorithm>

namespace EGE
{

void Scene::renderOnly(sf::RenderTarget& target)
{
    // The loop should NOT be specified for server-side
    // since it's NOT necessary (Scene itself is an EventLoop)
    ASSERT(m_loop);
    for(auto pr: *this)
    {
        pr.second->preRenderUpdate();
        pr.second->render(target);
    }
}

void Scene::onUpdate(long long tickCounter)
{
    EventLoop::onUpdate();
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        auto object = *it;
        object.second->onUpdate(tickCounter);
        if(object.second->isDead())
        {
            if(m_removeObjectCallback)
                m_removeObjectCallback(object.second);

            m_objects.erase(it);

            if(m_objects.empty())
                return;

            it = m_objects.find(object.first);
        }
    }
}

long long Scene::addObject(std::shared_ptr<SceneObject> object)
{
    if(!object->getObjectId())
    {
        // On server, give entities negative IDs to separate client and server objects.
        if(!getLoop())
            --m_greatestId;
        else
            ++m_greatestId;
        object->setObjectId(m_greatestId);
    }

    m_objects.insert(std::make_pair(object->getObjectId(), object));

    if(m_addObjectCallback)
        m_addObjectCallback(object);

    return object->getObjectId();
}

std::vector<SceneObject*> Scene::getObjects(std::function<bool(SceneObject*)> predicate)
{
    std::vector<SceneObject*> objects;
    for(ObjectMap::value_type it: m_objects)
    {
        if(predicate(it.second.get()))
        {
            objects.push_back(it.second.get());
        }
    }
    return objects;
}
std::vector<SceneObject*> Scene::getObjects(std::string typeId)
{
    return getObjects([typeId](SceneObject* object)->bool { return object->getId() == typeId; });
}
std::shared_ptr<SceneObject> Scene::getObject(long long id)
{
    auto it = m_objects.find(id);
    if(it != m_objects.end())
        return it->second;
    return nullptr;
}

}
