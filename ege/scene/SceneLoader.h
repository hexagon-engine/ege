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

    SceneLoader(SceneObjectCreatorRegistry& registry)
    : m_registry(registry) {}

    SharedPtr<ObjectMap> serializeScene(Scene& scene) const;
    bool deserializeScene(SharedPtr<ObjectMap> data, Scene& scene) const;

    bool saveScene(String fileName, Scene& scene, const IOStreamConverter& converter = JSONConverter()) const;
    bool loadScene(String fileName, Scene& scene, const IOStreamConverter& converter = JSONConverter()) const;

private:
    SceneObjectCreatorRegistry& m_registry;
};

}
