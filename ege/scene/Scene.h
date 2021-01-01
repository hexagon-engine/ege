/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/scene/SceneObject.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/scene/SceneLoader.h>
#include <ege/util/Converter.h>
#include <ege/util/JSONConverter.h>
#include <functional>
#include <map>
#include <memory>

#define SCENE_DEBUG 00

namespace EGE
{

class Scene : public ThreadSafeEventLoop, public Renderable
{
public:
    explicit Scene(GUIGameLoop* loop)
    : m_loop(loop) {}

    typedef IdMap<SharedPtr<SceneObject>> ObjectMap;

    bool loadfromFile(String saveFile, String sceneFile
                      const SceneLoader::SceneObjectCreatorRegistry& registry,
                      const IOStreamConverter& converter = JSONConverter());

    // TODO: Make this const !
    bool saveToFile(String saveFile, const SceneLoader::SceneObjectCreatorRegistry& registry, const IOStreamConverter& converter = JSONConverter());

    virtual void onUpdate(TickCount tickCounter);

    IdType addObject(std::shared_ptr<SceneObject> object);
    IdType addStaticObject(std::shared_ptr<SceneObject> object);

    std::vector<SceneObject*> getObjects(std::function<bool(SceneObject*)> predicate);
    std::vector<SceneObject*> getObjects(std::string typeId);
    std::shared_ptr<SceneObject> getObject(IdType id);
    std::shared_ptr<SceneObject> getStaticObject(IdType id);

    ObjectMap::const_iterator begin() const { return m_objects.begin(); }
    ObjectMap::const_iterator end() const { return m_objects.end(); }

    ObjectMap::iterator begin() { return m_objects.begin(); }
    ObjectMap::iterator end() { return m_objects.end(); }

    void setSize(Vec2d size) { m_size = size; }
    Vec2d getSize() const { return m_size; }

    void setAddObjectCallback(std::function<void(std::shared_ptr<SceneObject>)> func) { m_addObjectCallback = func; }
    void setRemoveObjectCallback(std::function<void(std::shared_ptr<SceneObject>)> func) { m_removeObjectCallback = func; }

    GUIGameLoop* getLoop() { return m_loop; }

    // We don't have GUI on servers!
    bool isHeadless() { return !getLoop(); }

protected:
    friend class SceneLoader;

    virtual void render(Renderer& renderer) const override;

    ObjectMap m_objects;
    ObjectMap m_staticObjects;

private:
    IdType m_greatestId = 0;
    IdType m_greatestStaticId = 0;
    Vec2d m_size;
    GUIGameLoop* m_loop;
    std::function<void(std::shared_ptr<SceneObject>)> m_addObjectCallback;
    std::function<void(std::shared_ptr<SceneObject>)> m_removeObjectCallback;
};

}
