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

#include "Scene.h"

#include "SceneLoader.h"

#include <algorithm>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>

namespace EGE
{

bool Scene::loadFromFile(String saveFile, String sceneFile,
                         const IOStreamConverter& converter)
{
    SceneLoader loader(*this);
    bool success = loader.loadSceneAndSave(saveFile, sceneFile, converter);

    return success;
}

bool Scene::saveToFile(String saveFile, const IOStreamConverter& converter)
{
    SceneLoader loader(*this);

    ege_log.debug() << "Objects: ";
    for(auto it: m_objects)
        printObject(it.second->serialize());
    ege_log.debug() << "Static Objects: ";
    for(auto it: m_staticObjects)
        printObject(it.second->serialize());

    return loader.saveScene(saveFile, converter);
}

void Scene::render(Renderer& renderer) const
{
    // The loop should NOT be specified for server-side
    // since it's NOT necessary (Scene itself is an EventLoop)
    ASSERT_WITH_MESSAGE(m_loop, "Cannot render on server-side");

    for(auto pr: m_objectsByLayer)
        pr.second->doRender(renderer);
}

void Scene::onUpdate(TickCount tickCounter)
{
    if(!isHeadless()) m_loop->getProfiler()->startSection("eventLoop");
    EventLoop::onUpdate();

    auto doUpdateForObjectMap = [this, tickCounter](Scene::ObjectMapType& objects, bool allowDead) {
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
                    ege_log.debug() << "SceneObject is dead: " << object.second->getObjectId() << " @" << object.second;
                    fire<RemoveObjectEvent>(*object.second);

                    // Set all children dead
                    if(object.second->m_children.size() > 0)
                    {
                        ege_log.debug() << "SceneObject is dead: Removing " << object.second->m_children.size() << " children of " << object.second;
                        for(auto& so: object.second->m_children)
                        {
                            so->setDead();
                        }
                    }

                    // Remove object from its parent's children list
                    if(object.second->m_parent)
                        object.second->m_parent->m_children.erase(object.second.get());

                    m_objectsByName.erase(object.second->getName());
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

UidType Scene::addObject(SharedPtr<SceneObject> object)
{
    if(!object)
        return 0;

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

    object->init();

    auto objIdDupe = m_objects.find(object->getObjectId());
    if(objIdDupe != m_objects.end())
    {
        // It's NOT normal!
        ege_log.critical() << "Duplicate SceneObject ID: " << object->getName() << " collides with " << objIdDupe->second->getName();
        return object->getObjectId();
    }
    if(m_objectsByName.find(object->getName()) != m_objectsByName.end())
    {
        // If it happens, fix your code!
        ege_log.crash() << "Duplicate SceneObject name: " << object->getName();
        CRASH_WITH_MESSAGE("Duplicate SceneObject name");
    }

    m_objects.insert(std::make_pair(object->getObjectId(), object));
    if(object->getName().empty())
        object->setName("SO" + std::to_string(object->getObjectId()));
    m_objectsByName.insert(std::make_pair(object->getName(), object.get()));

    fire<AddObjectEvent>(*object);
    // TODO: Do not rebuild layers if adding multiple objects in one tick
    rebuildLayers();
    return object->getObjectId();
}

UidType Scene::addStaticObject(SharedPtr<SceneObject> object, bool overwrite)
{
    if(!object)
        return 0;

    ASSERT_WITH_MESSAGE(!object->getName().empty(), "Static SceneObjects must have assigned name in scene data file");

    if(!object->getObjectId())
    {
        // On server, give entities negative IDs to separate client and server objects.
        if(!getLoop())
            --m_greatestStaticId;
        else
            ++m_greatestStaticId;
        object->setObjectId(m_greatestStaticId);
    }
    else
    {
        if(m_greatestStaticId < object->getObjectId())
            m_greatestStaticId = object->getObjectId();
    }

    object->init();

    if(m_staticObjects.find(object->getObjectId()) != m_staticObjects.end())
    {
        ege_log.critical() << "Duplicate SceneObject ID: " << object->getObjectId();
        return object->getObjectId();
    }
    auto it = m_objectsByName.find(object->getName());
    if(it != m_objectsByName.end())
    {
        // It's normal for static objects when static objects were saved!
        ege_log.verbose() << "Duplicate SceneObject name: " << object->getName();
        if(overwrite)
        {
            ege_log.debug() << "Scene::addObject(): overwriting " << it->second << " by " << object->getName();
            auto& oldObject = m_objectsByName[it->second->getName()];

            // Remove old object and add new (with new ID etc.)
            m_staticObjects.erase(oldObject->getObjectId());
            m_staticObjects.insert(std::make_pair(object->getObjectId(), object));

            // Set another 'object by name'.
            oldObject = object.get();
        }
        return object->getObjectId();
    }

    m_staticObjects.insert(std::make_pair(object->getObjectId(), object));
    m_objectsByName.insert(std::make_pair(object->getName(), object.get()));
    // TODO: Do not rebuild layers if adding multiple objects in one tick
    rebuildLayers();
    return object->getObjectId();
}

std::vector<SceneObject*> Scene::getObjects(std::function<bool(SceneObject*)> predicate)
{
    std::vector<SceneObject*> objects;
    for(ObjectMapType::value_type it: m_objects)
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
    return getObjects([typeId](SceneObject* object)->bool { return object->getType()->getId() == typeId; });
}

SharedPtr<SceneObject> Scene::getObject(UidType id)
{
    auto it = m_objects.find(id);
    if(it != m_objects.end())
        return it->second;
    return nullptr;
}

SharedPtr<SceneObject> Scene::getStaticObject(UidType id)
{
    auto it = m_staticObjects.find(id);
    if(it != m_staticObjects.end())
        return it->second;
    return nullptr;
}

SceneObject* Scene::getObjectByName(String id)
{
    auto it = m_objectsByName.find(id);
    if(it != m_objectsByName.end())
        return it->second;
    return nullptr;
}

SharedPtr<SceneObject> Scene::addNewObject(String typeId, SharedPtr<ObjectMap> data)
{
    SharedPtr<SceneObject> sceneObject = createObject(typeId, data);
    addObject(sceneObject);
    return sceneObject;
}

SharedPtr<SceneObject> Scene::addNewStaticObject(String typeId, SharedPtr<ObjectMap> data)
{
    SharedPtr<SceneObject> sceneObject = createObject(typeId, data);
    addStaticObject(sceneObject);
    return sceneObject;
}

SharedPtr<SceneObject> Scene::createObject(String typeId, SharedPtr<ObjectMap> data)
{
    auto registry = getRegistry();

    auto type = registry.getType(typeId);
    if(!type)
    {
        ege_log.warning() << "Invalid SceneObjectType: " << typeId;
        return nullptr;
    }

    SharedPtr<SceneObject> sceneObject = type->createEmptyObject(*this);
    if(!sceneObject)
    {
        ege_log.error() << "Object refused creation!";
        return nullptr;
    }
    sceneObject->setType(type);

    if(!data)
        return sceneObject;

    if(!sceneObject->deserialize(data))
    {
        ege_log.error() << "Failed to deserialize SceneObject!";
        return nullptr;
    }

    return sceneObject;
}

void Scene::rebuildLayers()
{
    m_objectsByLayer.clear();

    for(auto pr: m_staticObjects)
        m_objectsByLayer.insert(std::make_pair(pr.second->getRenderLayer(), pr.second.get()));

    for(auto pr: m_objects)
        m_objectsByLayer.insert(std::make_pair(pr.second->getRenderLayer(), pr.second.get()));
}

}
