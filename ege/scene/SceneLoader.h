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

#pragma once

#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/util/Converter.h>
#include <ege/util/Types.h>
#include <ege/scene/SceneObject.h>
#include <functional>

#define EGE_SCENE2D_OBJECT_CREATOR(clazz) [](EGE::Scene& scene) { return make<clazz>((EGE::Scene2D&)scene); }

namespace EGE
{

class SceneLoader
{
public:
    typedef std::function<SharedPtr<SceneObject>(Scene&)> SceneObjectCreator;

    class SceneObjectCreatorRegistry : public GameplayObjectRegistry<String, SceneObjectCreator>
    {
    public:
        RegistryError addEntry(String typeId, SceneObjectCreator creator)
        {
            return add(typeId, std::make_unique<SceneObjectCreator>(creator));
        }
    };

    SceneLoader(Scene& scene, const SceneObjectCreatorRegistry& registry)
    : m_scene(scene), m_registry(registry) {}

    SharedPtr<ObjectMap> serializeSceneObjects() const;
    bool deserializeSceneObjects(SharedPtr<ObjectMap> data);
    bool deserializeStaticSceneObjects(SharedPtr<ObjectMap> data);

    // Used for game saves
    bool saveScene(String fileName, const IOStreamConverter& converter = JSONConverter()) const;
    bool loadScene(String fileName, const IOStreamConverter& converter = JSONConverter());

    // Used for predefined scenes
    bool loadStaticObjects(String fileName, const IOStreamConverter& converter = JSONConverter());

    // Load save and predefined scene at once, in proper order
    // (that allows overwriting predefined objects with save
    // objects if ID's are duplicated).
    bool loadSceneAndSave(String saveName, String sceneName, const IOStreamConverter& converter = JSONConverter());

private:
    SharedPtr<SceneObject> loadObject(Optional<SharedPtr<ObjectMap>> objMap);

    Scene& m_scene;
    const SceneObjectCreatorRegistry& m_registry;
};

}
