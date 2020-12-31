/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/scene/Scene.h>
#include <ege/util/Converter.h>
#include <ege/util/Types.h>

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

    SceneLoader(Scene& scene, SceneObjectCreatorRegistry& registry)
    : m_scene(scene), m_registry(registry) {}

    SharedPtr<ObjectMap> serializeSceneObjects() const;
    bool deserializeSceneObjects(SharedPtr<ObjectMap> data);
    bool deserializeStaticSceneObjects(SharedPtr<ObjectMap> data);

    // Used for game saves
    bool saveScene(String fileName, const IOStreamConverter& converter = JSONConverter()) const;
    bool loadScene(String fileName, const IOStreamConverter& converter = JSONConverter());

    // Used for predefined scenes
    bool loadStaticObjects(String fileName, const IOStreamConverter& converter = JSONConverter());

private:
    SharedPtr<SceneObject> loadObject(Optional<SharedPtr<ObjectMap>> objMap);

    Scene& m_scene;
    SceneObjectCreatorRegistry& m_registry;
};

}
