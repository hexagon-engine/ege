/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneLoader.h"

#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>

#include <fstream>

namespace EGE
{

SharedPtr<ObjectMap> SceneLoader::serializeScene(Scene& scene) const
{
    auto data = make<ObjectMap>();
    auto objects = make<ObjectList>();

    for(auto& sObj : scene)
    {
        auto entry = sObj.second->serialize();
        entry->addInt("id", sObj.second->getObjectId());
        entry->addString("typeId", sObj.second->getId());
        objects->addObject(entry);
    }
    data->addObject("objects", objects);
    return data;
}

bool SceneLoader::deserializeScene(SharedPtr<ObjectMap> data, Scene& scene) const
{
    auto objects = data->getObject("objects").to<ObjectList>();
    if(!objects.hasValue())
        return false;

    for(auto obj: *objects.value())
    {
        auto objMap = Object::cast<ObjectMap>(obj);
        if(!objMap.hasValue())
        {
            err() << "SceneObject description is not a Map!";
            continue;
        }

        auto typeId = objMap.value()->getObject("typeId").as<String>();
        auto numId = objMap.value()->getObject("id").as<MaxInt>();
        if(!typeId.hasValue())
        {
            err() << "SceneObject has no valid type ID!";
            continue;
        }
        if(!numId.hasValue())
        {
            err() << "SceneObject has no valid ID!";
            continue;
        }

        auto creator = m_registry.findById(typeId.value());
        if(!creator)
        {
            err() << "No SOC found for " << typeId.value();
            continue;
        }

        SharedPtr<SceneObject> sceneObject = (*creator)(scene);
        sceneObject->setObjectId(numId.value());

        if(!sceneObject->deserialize(objMap.value()))
        {
            err() << "Failed to deserialize SceneObject!";
            continue;
        }

        scene.addObject(sceneObject);
    }

    return true;
}

bool SceneLoader::saveScene(String fileName, Scene& scene, const IOStreamConverter& converter) const
{
    log() << "Saving scene to " << fileName;
    std::ofstream file(fileName);
    if(!file.good())
    {
        err() << "Scene saving failed - failed to open file!";
        return false;
    }

    auto object = serializeScene(scene);
    if(!object)
    {
        err() << "Scene loading failed - failed to generate data!";
        return false;
    }

    return converter.out(file, *object);
}

bool SceneLoader::loadScene(String fileName, Scene& scene, const IOStreamConverter& converter) const
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

    return deserializeScene(objectMap.value(), scene);
}

}
