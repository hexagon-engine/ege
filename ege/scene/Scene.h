/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/gui/Widget.h>
#include <ege/scene/SceneObject.h>
#include <functional>
#include <map>
#include <memory>

#define SCENE_DEBUG 1

namespace EGE
{

class Scene : public Widget
{
public:
    Scene(Widget* parent)
    : Widget(parent) {}

    typedef std::map<long long, std::shared_ptr<SceneObject>> ObjectMap;

    virtual void render(sf::RenderTarget& target);
    virtual void onUpdate(long long tickCounter);
    virtual void onResize(sf::Event::SizeEvent& event);

    long long addObject(std::shared_ptr<SceneObject> object);

    SceneObject* getObject(std::function<bool(SceneObject*)> predicate);
    SceneObject* getObject(std::string name);
    SceneObject* getObject(long long id);

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
        m_autoResizable = false;
    }
    sf::Vector2f getSize()
    {
        return m_size;
    }

    // The Scene widget is set to be auto resizable if its size is not set
    // before first render. You can revert this behaviour by using setSize().
    virtual void setViewForWidget(sf::RenderTarget& target);

private:
    ObjectMap m_objects;
    bool m_autoResizable = false;
    long long m_greatestId = 0;
};

}
