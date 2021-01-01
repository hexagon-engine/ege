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

#include "SceneLoader.h"

#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/scene/Scene.h>

#include <fstream>

namespace EGE
{

SharedPtr<ObjectMap> SceneLoader::serializeSceneObjects() const
{
    auto data = make<ObjectMap>();

    // Add dynamic objects
    auto objects = make<ObjectList>();
    for(auto& sObj : m_scene.m_objects)
    {
        auto entry = sObj.second->serialize();
        entry->addInt("id", sObj.second->getObjectId());
        entry->addString("typeId", sObj.second->getId());
        objects->addObject(entry);
    }
    data->addObject("objects", objects);

    // Add static objects (if changed)
    auto staticObjects = make<ObjectList>();
    for(auto& sObj : m_scene.m_staticObjects)
    {
        if(sObj.second->didChangeSinceLoad())
        {
            auto entry = sObj.second->serialize();
            entry->addInt("id", sObj.second->getObjectId());
            entry->addString("typeId", sObj.second->getId());
            staticObjects->addObject(entry);
        }
    }
    data->addObject("staticObjects", staticObjects);

    return data;
}

SharedPtr<SceneObject> SceneLoader::loadObject(Optional<SharedPtr<ObjectMap>> objMap)
{
    if(!objMap.hasValue())
    {
        err() << "SceneObject description is not a Map!";
        return nullptr;
    }

    auto typeId = objMap.value()->getObject("typeId").as<String>();
    auto numId = objMap.value()->getObject("id").as<MaxInt>();
    if(!typeId.hasValue())
    {
        err() << "SceneObject has no valid type ID!";
        return nullptr;
    }
    if(!numId.hasValue())
    {
        err() << "SceneObject has no valid ID!";
        return nullptr;
    }

    auto creator = m_registry.findById(typeId.value());
    if(!creator)
    {
        log(LogLevel::Warning) << "No SOC found for " << typeId.value();
        return nullptr;
    }

    SharedPtr<SceneObject> sceneObject = (*creator)(m_scene);
    sceneObject->setObjectId(numId.value());

    if(!sceneObject->deserialize(objMap.value()))
    {
        err() << "Failed to deserialize SceneObject!";
        return nullptr;
    }

    return sceneObject;
}

bool SceneLoader::deserializeSceneObjects(SharedPtr<ObjectMap> data)
{
    // Load all static objects that changed from installation scene
    auto staticObjects = data->getObject("staticObjects").to<ObjectList>();
    if(!staticObjects.hasValue())
        return false;

    for(auto obj: *staticObjects.value())
    {
        auto objMap = Object::cast<ObjectMap>(obj);
        auto sceneObject = loadObject(objMap);

        if(!sceneObject)
            continue;

        m_scene.addStaticObject(sceneObject);
    }

    // Load all "dynamic" objects
    auto objects = data->getObject("objects").to<ObjectList>();
    if(!objects.hasValue())
        return false;

    for(auto obj: *objects.value())
    {
        auto objMap = Object::cast<ObjectMap>(obj);
        auto sceneObject = loadObject(objMap);

        if(!sceneObject)
            continue;

        m_scene.addObject(sceneObject);
    }

    return true;
}

bool SceneLoader::deserializeStaticSceneObjects(SharedPtr<ObjectMap> data)
{
    // Load all objects and add them as static objects.
    auto objects = data->getObject("objects").to<ObjectList>();
    if(!objects.hasValue())
        return false;

    for(auto obj: *objects.value())
    {
        auto objMap = Object::cast<ObjectMap>(obj);
        auto sceneObject = loadObject(objMap);

        if(!sceneObject)
            continue;

        m_scene.addStaticObject(sceneObject);
    }

    return true;
}

bool SceneLoader::saveScene(String fileName, const IOStreamConverter& converter) const
{
    log() << "Saving scene to " << fileName;
    std::ofstream file(fileName);
    if(!file.good())
    {
        err() << "Scene saving failed - failed to open file!";
        return false;
    }

    auto object = serializeSceneObjects();
    if(!object)
    {
        err() << "Scene loading failed - failed to generate data!";
        return false;
    }

    return converter.out(file, *object);
}

bool SceneLoader::loadScene(String fileName, const IOStreamConverter& converter)
{
    log() << "Loading scene from " << fileName;
    std::ifstream file(fileName);
    if(!file.good())
    {
        err() << "Scene loading failed - failed to open file!";
        return false;
    }

    SharedPtr<Object> object;
    if(!converter.in(file, object))
    {
        err() << "Scene loading failed - failed to parse file data!";
        return false;
    }

    auto objectMap = object->cast<ObjectMap>(object);
    if(!objectMap.hasValue())
    {
        err() << "Scene loading failed - invalid data format!";
        return false;
    }

    if(!deserializeSceneObjects(objectMap.value()))
    {
        err() << "Scene loading failed - failed to load data!";
        return false;
    }
    return true;
}

bool SceneLoader::loadStaticObjects(String fileName, const IOStreamConverter& converter)
{
    log() << "Loading static objects from " << fileName;
    std::ifstream file(fileName);
    if(!file.good())
    {
        err() << "Static object loading failed - failed to open file!";
        return false;
    }

    SharedPtr<Object> object;
    if(!converter.in(file, object))
    {
        err() << "Static object loading failed - failed to parse file data!";
        return false;
    }

    auto objectMap = object->cast<ObjectMap>(object);
    if(!objectMap.hasValue())
    {
        err() << "Static object loading failed - invalid data format!";
        return false;
    }

    if(!deserializeStaticSceneObjects(objectMap.value()))
    {
        err() << "Static object loading failed - failed to load data!";
        return false;
    }
    return true;
}

bool SceneLoader::loadSceneAndSave(String saveName, String sceneName, const IOStreamConverter& converter)
{
    if(!loadScene(saveName, converter))
        // It's nothing wrong, we just create a new save!
        log(LogLevel::Warning) << "Empty or invalid save: " << saveName;

    return loadStaticObjects(sceneName, converter);;
}

}
