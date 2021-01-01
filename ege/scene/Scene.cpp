/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene.h"

#include "SceneLoader.h"

#include <algorithm>
#include <ege/debug/Logger.h>

namespace EGE
{

bool Scene::loadfromFile(String saveFile, String sceneFile,
                         const SceneLoader::SceneObjectCreatorRegistry& registry,
                         const IOStreamConverter& converter)
{
    SceneLoader loader(*this, registry);
    return loader.loadSceneAndSave(saveFile, sceneFile, converter);
}

bool Scene::saveToFile(String saveFile, const SceneLoader::SceneObjectCreatorRegistry& registry, const IOStreamConverter& converter)
{
    SceneLoader loader(*this, registry);
    return loader.saveScene(saveFile, converter);
}

void Scene::render(Renderer& renderer) const
{
    // The loop should NOT be specified for server-side
    // since it's NOT necessary (Scene itself is an EventLoop)
    ASSERT_WITH_MESSAGE(m_loop, "Cannot render on server-side");

    for(auto pr: m_staticObjects)
    {
        pr.second->doRender(renderer);
    }

    for(auto pr: m_objects)
    {
        pr.second->doRender(renderer);
    }
}

void Scene::onUpdate(TickCount tickCounter)
{
    if(!isHeadless()) m_loop->getProfiler()->startSection("eventLoop");
    EventLoop::onUpdate();

    auto doUpdateForObjectMap = [this, tickCounter](Scene::ObjectMap& objects, bool allowDead) {
        for(auto it = objects.begin(); it != objects.end();)
        {
            if(!isHeadless()) m_loop->getProfiler()->startSection("update");
            auto object = *it;
            auto oldIt = it;
            auto nextIt = ++it;
            object.second->onUpdate(tickCounter);

            if(allowDead)
            {
                if(!isHeadless()) m_loop->getProfiler()->endStartSection("deadCheck");
                if(object.second->isDead())
                {
                    log(LogLevel::Debug) << "SceneObject is dead: " << object.second->getObjectId() << " @" << object.second;
                    if(m_removeObjectCallback)
                        m_removeObjectCallback(object.second);

                    objects.erase(oldIt);

                    if(objects.empty())
                        return;
                }
            }
            it = nextIt;
            if(!isHeadless()) m_loop->getProfiler()->endSection();
        }
    };

    if(!isHeadless()) m_loop->getProfiler()->endStartSection("objectUpdate");
    doUpdateForObjectMap(m_objects, true);

    if(!isHeadless()) m_loop->getProfiler()->endStartSection("staticObjectUpdate");
    doUpdateForObjectMap(m_staticObjects, false);

    if(!isHeadless()) m_loop->getProfiler()->endSection();
}

IdType Scene::addObject(std::shared_ptr<SceneObject> object)
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
    else
    {
        if(m_greatestId < object->getObjectId())
            m_greatestId = object->getObjectId();
    }

    if(m_objects.find(object->getObjectId()) != m_objects.end())
    {
        // It's NOT normal!
        log(LogLevel::Error) << "Duplicate SceneObject ID: " << object->getObjectId();
        return object->getObjectId();
    }

    m_objects.insert(std::make_pair(object->getObjectId(), object));

    if(m_addObjectCallback)
        m_addObjectCallback(object);

    return object->getObjectId();
}

IdType Scene::addStaticObject(std::shared_ptr<SceneObject> object)
{
    ASSERT_WITH_MESSAGE(object->getObjectId(), "Static SceneObjects must have assigned ID in scene data file");
    if(m_greatestStaticId < object->getObjectId())
        m_greatestStaticId = object->getObjectId();

    if(m_staticObjects.find(object->getObjectId()) != m_staticObjects.end())
    {
        // It's normal for static objects when static objects were saved!
        log(LogLevel::Verbose) << "Duplicate SceneObject ID: " << object->getObjectId();
        return object->getObjectId();
    }

    m_staticObjects.insert(std::make_pair(object->getObjectId(), object));
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
std::shared_ptr<SceneObject> Scene::getObject(IdType id)
{
    auto it = m_objects.find(id);
    if(it != m_objects.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<SceneObject> Scene::getStaticObject(IdType id)
{
    auto it = m_staticObjects.find(id);
    if(it != m_staticObjects.end())
        return it->second;
    return nullptr;
}

}
