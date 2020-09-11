/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/scene/SceneObject.h>
#include <ege/gui/GUIGameLoop.h>
#include <functional>
#include <map>
#include <memory>

#define SCENE_DEBUG 1

namespace EGE
{

class Scene : public ThreadSafeEventLoop
{
public:
    explicit Scene(GUIGameLoop* loop)
    : m_loop(loop) {}

    typedef std::map<long long, std::shared_ptr<SceneObject>> ObjectMap;

    virtual void renderOnly(sf::RenderTarget& target);

    virtual void onUpdate(long long tickCounter);

    long long addObject(std::shared_ptr<SceneObject> object);

    std::vector<SceneObject*> getObjects(std::function<bool(SceneObject*)> predicate);
    std::vector<SceneObject*> getObjects(std::string typeId);
    std::shared_ptr<SceneObject> getObject(long long id);

    ObjectMap::const_iterator begin()
    {
        return m_objects.begin();
    }
    ObjectMap::const_iterator end()
    {
        return m_objects.end();
    }

    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    sf::Vector2f getSize()
    {
        return m_size;
    }

    void setAddObjectCallback(std::function<void(std::shared_ptr<SceneObject>)> func)
    {
        m_addObjectCallback = func;
    }

    void setRemoveObjectCallback(std::function<void(std::shared_ptr<SceneObject>)> func)
    {
        m_removeObjectCallback = func;
    }

    GUIGameLoop* getLoop()
    {
        return m_loop;
    }

protected:
    ObjectMap m_objects;

private:
    long long m_greatestId = 0;
    sf::Vector2f m_size;
    GUIGameLoop* m_loop;
    std::function<void(std::shared_ptr<SceneObject>)> m_addObjectCallback;
    std::function<void(std::shared_ptr<SceneObject>)> m_removeObjectCallback;
};

}
