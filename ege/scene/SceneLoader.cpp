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

bool SceneLoader::loadRegistry(SceneObjectRegistry& registry, String fileName, const IOStreamConverter& converter)
{
    SharedPtr<Object> data;
    std::ifstream file(CommonPaths::resourceDir() + "/" + fileName);
    if(!file.good())
    {
        ege_log.error() << "Failed to open registry file: " << fileName;
        return false; // couldn't open file
    }

    if(!(file >> objectIn(data, converter)))
    {
        ege_log.error() << "Invalid registry JSON";
        return false; // invalid JSON
    }

    auto data_map = Object::cast<ObjectMap>(data);
    if(!data_map.hasValue())
    {
        ege_log.error() << "Data must be a map";
        return false; // it's not a map!
    }

    for(auto& pr: *data_map.value())
    {
        auto sodata = Object::cast<ObjectMap>(pr.second);
        if(!sodata.hasValue())
        {
            ege_log.error() << "Data entry '" << pr.first << "' must be a map";
            return false; // entry not a map
        }

        SharedPtr<SceneObjectType> sotype = registry.getType(pr.first);
        if(!sotype)
        {
            ege_log.verbose() << "SceneLoader: Creating generic type for " << pr.first;
            sotype = make<SceneObjectType>(pr.first);
        }
        else
            ege_log.verbose() << "SceneLoader: Using already registered type for " << pr.first;

        // include key has priority. If there are both keys, 'data' key is ignored.
        SharedPtr<ObjectMap> sd_data;
        auto sd_include = sodata.value()->get("include").asString();
        if(sd_include.hasValue())
        {
            // Try to load from specified file
            String fileName = CommonPaths::resourceDir() + "/" + sd_include.value();
            std::fstream input(fileName);
            if(!input.good())
            {
                ege_log.error() << "Failed to open '" << fileName << "' required by SceneObjectType '" << pr.first << "'";
                return false;
            }
            SharedPtr<Object> data;
            if(!(input >> objectIn(data, converter)))
            {
                ege_log.error() << "Failed to load data from '" << fileName << "' required by SceneObjectType '" << pr.first << "'";
                return false;
            }
            sd_data = Object::cast<ObjectMap>(data).valueOr({});
            if(!sd_data)
            {
                ege_log.error() << "Invalid format of data loaded from " << fileName << ", must be a map";
                return false;
            }
        }
        else
        {
            // Try to load inlined
            auto data = sodata.value()->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
            {
                ege_log.error() << "A registry entry '" << pr.first << "' must have 'data' or 'include' key";
                return false;
            }
            sd_data = data.value();
        }

        if(!sotype->deserialize(sd_data))
        {
            ege_log.error() << "Failed to deserialize SceneObjectType '" << pr.first << "'";
            return false;
        }
        registry.addType(sotype);
    }
    return true;
}

SharedPtr<ObjectMap> SceneLoader::serializeSceneObjects() const
{
    auto data = make<ObjectMap>();

    // Add dynamic objects
    auto objects = make<ObjectList>();
    for(auto& sObj : m_scene.m_objects)
    {
        if(!sObj.second->allowSave())
            continue;

        auto entry = sObj.second->serialize();
        entry->addString("typeId", sObj.second->getType()->getId());
        objects->addObject(entry);
    }
    ege_log.debug() << "SceneLoader finished saving " << objects->size() << " objects";
    data->addObject("objects", objects);

    // Add static objects (if changed)
    auto staticObjects = make<ObjectList>();
    for(auto& sObj : m_scene.m_staticObjects)
    {
        if(!sObj.second->allowSave())
            continue;

        if(sObj.second->didChangeSinceLoad())
        {
            auto entry = sObj.second->serialize();
            entry->addString("typeId", sObj.second->getType()->getId());
            staticObjects->addObject(entry);
        }
    }
    ege_log.debug() << "SceneLoader finished saving " << staticObjects->size() << " static objects";
    data->addObject("staticObjects", staticObjects);

    return data;
}

SharedPtr<SceneObject> SceneLoader::loadObject(Optional<SharedPtr<ObjectMap>> objMap)
{
    if(!objMap.hasValue())
    {
        ege_log.error() << "SceneObject description is not a Map!";
        return nullptr;
    }

    auto typeId = objMap.value()->getObject("typeId").asString();
    if(!typeId.hasValue())
    {
        ege_log.error() << "SceneObject has no valid type ID!";
        return nullptr;
    }

    return m_scene.createObject(typeId.value(), objMap.value());
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

        m_scene.addStaticObject(sceneObject, true);
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

    ege_log.debug() << "SceneLoader finished loading with " << m_scene.m_objects.size() << " objects";

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

    ege_log.debug() << "SceneLoader finished loading with " << m_scene.m_staticObjects.size() << " static objects";

    return true;
}

bool SceneLoader::saveScene(String fileName, const IOStreamConverter& converter) const
{
    ege_log.info() << "Saving scene to " << fileName;

    if(!EGE::System::createPath(CommonPaths::saveDir()))
    {
        ege_log.warning() << "Scene saving failed - failed to create path!";
        return false;
    }

    std::ofstream file(CommonPaths::saveDir() + "/" + fileName);
    if(!file.good())
    {
        ege_log.error() << "Scene saving failed - failed to open file!";
        return false;
    }

    auto object = serializeSceneObjects();
    if(!object)
    {
        ege_log.error() << "Scene saving failed - failed to generate data!";
        return false;
    }

    return converter.out(file, *object);
}

bool SceneLoader::loadScene(String fileName, const IOStreamConverter& converter)
{
    ege_log.info() << "Loading scene from " << CommonPaths::saveDir() + "/" + fileName;

    std::ifstream file(CommonPaths::saveDir() + "/" + fileName);
    if(!file.good())
    {
        ege_log.warning() << "Scene loading failed - failed to open file!";
        return false;
    }

    SharedPtr<Object> object;
    if(!converter.in(file, object))
    {
        ege_log.error() << "Scene loading failed - failed to parse file data!";
        return false;
    }

    auto objectMap = object->cast<ObjectMap>(object);
    if(!objectMap.hasValue())
    {
        ege_log.error() << "Scene loading failed - invalid data format!";
        return false;
    }

    if(!deserializeSceneObjects(objectMap.value()))
    {
        ege_log.error() << "Scene loading failed - failed to load data!";
        return false;
    }
    return true;
}

bool SceneLoader::loadStaticObjects(String fileName, const IOStreamConverter& converter)
{
    ege_log.info() << "Loading static objects from " << CommonPaths::resourceDir() + "/" + fileName;

    std::ifstream file(CommonPaths::resourceDir() + "/" + fileName);
    if(!file.good())
    {
        ege_log.error() << "Static object loading failed - failed to open file!";
        return false;
    }

    SharedPtr<Object> object;
    if(!converter.in(file, object))
    {
        ege_log.error() << "Static object loading failed - failed to parse file data!";
        return false;
    }

    auto objectMap = object->cast<ObjectMap>(object);
    if(!objectMap.hasValue())
    {
        ege_log.error() << "Static object loading failed - invalid data format!";
        return false;
    }

    if(!deserializeStaticSceneObjects(objectMap.value()))
    {
        ege_log.error() << "Static object loading failed - failed to load data!";
        return false;
    }
    return true;
}

bool SceneLoader::loadSceneAndSave(String saveName, String sceneName, const IOStreamConverter& converter)
{
    if(sceneName.empty())
        return true;

    if(!loadStaticObjects(sceneName, converter))
    {
        ege_log.critical() << "Failed to load predefined scene!";
        return false;
    }

    if(saveName.empty())
        return true;

    if(!loadScene(saveName, converter))
        // It's nothing wrong, we will just create a new save!
        ege_log.warning() << "Empty or invalid save: " << saveName;

    return true;
}

}
