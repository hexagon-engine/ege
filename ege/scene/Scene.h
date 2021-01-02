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
#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/scene/SceneLoader.h>
#include <ege/scene/SceneObject.h>
#include <ege/util/Converter.h>
#include <ege/util/JSONConverter.h>
#include <functional>
#include <map>
#include <memory>

#define SCENE_DEBUG 0

namespace EGE
{

class Scene : public ThreadSafeEventLoop, public Renderable
{
public:
    explicit Scene(GUIGameLoop* loop)
    : m_loop(loop) {}

    typedef IdMap<SharedPtr<SceneObject>> ObjectMap;
    typedef StringMap<SceneObject*> ObjectMapByName;

    bool loadFromFile(String saveFile, String sceneFile,
                      const SceneLoader::SceneObjectCreatorRegistry& registry,
                      const IOStreamConverter& converter = JSONConverter());

    // TODO: Make this const !
    bool saveToFile(String saveFile, const SceneLoader::SceneObjectCreatorRegistry& registry, const IOStreamConverter& converter = JSONConverter());

    virtual void onUpdate(TickCount tickCounter);

    // %overwrite - overwrite object instead of skipping when name conflict arises
    IdType addObject(std::shared_ptr<SceneObject> object);
    IdType addStaticObject(std::shared_ptr<SceneObject> object, bool overwrite = false);

    std::vector<SceneObject*> getObjects(std::function<bool(SceneObject*)> predicate);
    std::vector<SceneObject*> getObjects(std::string typeId);

    std::shared_ptr<SceneObject> getObject(IdType id);
    std::shared_ptr<SceneObject> getStaticObject(IdType id);

    SceneObject* getObjectByName(String name);

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
    ObjectMapByName m_objectsByName;

private:
    IdType m_greatestId = 0;
    IdType m_greatestStaticId = 0;
    Vec2d m_size;
    GUIGameLoop* m_loop;
    std::function<void(std::shared_ptr<SceneObject>)> m_addObjectCallback;
    std::function<void(std::shared_ptr<SceneObject>)> m_removeObjectCallback;
};

}
