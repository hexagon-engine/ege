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

#include <SFML/Graphics.hpp>

#include <ege/gfx/RenderStates.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/scene/SceneLoader.h>
#include <ege/scene/SceneObject.h>
#include <ege/scene/Camera.h>
#include <ege/util/Converter.h>
#include <ege/util/JSONConverter.h>
#include <functional>
#include <map>
#include <memory>
#include <mutex>

#define SCENE_DEBUG 0

namespace EGE
{

class ObjectEvent : public Event
{
public:
    ObjectEvent(SceneObject& _object)
    : object(_object) {}

    SceneObject& object;
};

class AddObjectEvent : public ObjectEvent
{
public:
    AddObjectEvent(SceneObject& _object)
    : ObjectEvent(_object) {}

    EGE_EVENT("AddObjectEvent");
};

class RemoveObjectEvent : public ObjectEvent
{
public:
    RemoveObjectEvent(SceneObject& _object)
    : ObjectEvent(_object) {}

    EGE_EVENT("RemoveObjectEvent");
};


class Scene : public Component<SceneObject>, public Renderable
{
public:
    Scene(SharedPtr<SceneObjectRegistry> registry = nullptr);
    Scene(GUIGameLoop& loop, SharedPtr<SceneObjectRegistry> registry = nullptr);
    virtual ~Scene();

    typedef IdMap<SharedPtr<SceneObject>> ObjectMapType;
    typedef StringMap<SceneObject*> ObjectMapByName;
    typedef std::multimap<int, SceneObject*> ObjectMapByLayer;

    bool loadFromFile(String saveFile, String sceneFile, const IOStreamConverter& converter = JSONConverter());

    // TODO: Make this const !
    bool saveToFile(String saveFile, const IOStreamConverter& converter = JSONConverter());

    virtual void onTick() override;

    // %overwrite - overwrite object instead of skipping when name conflict arises
    UidType addObject(SharedPtr<SceneObject> const& object);
    UidType addStaticObject(SharedPtr<SceneObject> const& object, bool overwrite = false);

    // Custom object
    template<class SO>
    SharedPtr<SO> addNewObject(SharedPtr<ObjectMap> data = nullptr)
    {
        static_assert(std::is_base_of_v<SceneObject, SO>);
        SharedPtr<SO> sceneObject = SceneObject::cast<SO>(createObject(SO::type(), data));
        ASSERT(sceneObject);
        addObject(sceneObject);
        return sceneObject;
    }

    template<class SO>
    SharedPtr<SO> addNewStaticObject(SharedPtr<ObjectMap> data = nullptr)
    {
        static_assert(std::is_base_of_v<SceneObject, SO>);
        SharedPtr<SO> sceneObject = SceneObject::cast<SO>(createObject(SO::type(), data));
        ASSERT(sceneObject);
        addStaticObject(sceneObject);
        return sceneObject;
    }

    // Objects loaded from file
    SharedPtr<SceneObject> addNewObject(String typeId, SharedPtr<ObjectMap> data = nullptr);
    SharedPtr<SceneObject> addNewStaticObject(String typeId, SharedPtr<ObjectMap> data = nullptr);

    // NOTE: This shouldn't really be called directly, but if someone wants...
    SharedPtr<SceneObject> createObject(String typeId, SharedPtr<ObjectMap> data);

    template<class SO>
    SharedPtr<SO> createObject(SharedPtr<ObjectMap> data)
    {
        static_assert(std::is_base_of_v<SceneObject, SO>);
        return SceneObject::cast<SO>(createObject(SO::type(), data));
    }

    Vector<SceneObject*> getObjects(std::function<bool(SceneObject*)> predicate);
    Vector<SceneObject*> getObjects(std::string typeId);

    SharedPtr<SceneObject> getObject(UidType id);
    SharedPtr<SceneObject> getStaticObject(UidType id);

    SceneObject* getObjectByName(String name);

    template<class SO>
    SO* getObjectByName(String const& name)
    {
        static_assert(std::is_base_of_v<SceneObject, SO>);
        return dynamic_cast<SO*>(getObjectByName(name));
    }

    ObjectMapType::const_iterator begin() const { return m_objects.begin(); }
    ObjectMapType::const_iterator end() const { return m_objects.end(); }

    ObjectMapType::iterator begin() { return m_objects.begin(); }
    ObjectMapType::iterator end() { return m_objects.end(); }

    void setSize(Vec2d size) { m_size = size; }
    Vec2d getSize() const { return m_size; }

    GUIGameLoop* getLoop() const { return m_loop; }

    // We don't have GUI on servers!
    bool isHeadless() const { return !getLoop(); }

    // The registry is guaranteed to exist since it is created in
    // constructor if nullptr is given.
    const SceneObjectRegistry& getRegistry() const { return *m_registry; }
    SceneObjectRegistry& getRegistry() { return *m_registry; }

    virtual void doRender(Renderer& renderer, const RenderStates& states = {}) override;

    // Get camera that is currently used for rendering. Returns nullptr if not rendering.
    Camera const* getCurrentCamera() const { return m_currentCamera; }

protected:
    friend class SceneLoader;
    friend class SceneWidget;

    virtual void render(Renderer& renderer) const override;
    virtual void rebuildLayers();

    virtual void forEachChildImpl(_ForEachChildCallbackBase&) override;

    ObjectMapType m_objects;
    ObjectMapType m_staticObjects;
    ObjectMapByName m_objectsByName;
    ObjectMapByLayer m_objectsByLayer;
    std::recursive_mutex m_objectsMutex;

    SharedPtr<SceneObjectRegistry> m_registry = nullptr;
    String m_lastLoadFile;

private:
    UidType m_greatestId = 0;
    UidType m_greatestStaticId = 0;
    Vec2d m_size;
    GUIGameLoop* m_loop;
    Camera* m_currentCamera = nullptr;
};

}
