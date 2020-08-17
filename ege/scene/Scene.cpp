/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene.h"

#include <algorithm>

namespace EGE
{

void Scene::render(sf::RenderTarget& target)
{
    Widget::render(target);
    for(auto pr: *this)
    {
        pr.second->render(target);
    }
}

void Scene::onResize(sf::Event::SizeEvent& event)
{
    if(m_autoResizable)
        m_size = sf::Vector2f(event.width, event.height);
}

void Scene::onUpdate(long long tickCounter)
{
    Widget::onUpdate(tickCounter);
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        auto object = *it;
        object.second->onUpdate(tickCounter);
        if(object.second->isDead())
        {
            m_objects.erase(it);

            if(m_objects.empty())
                return;

            it = m_objects.find(object.first);
        }
    }
}

long long Scene::addObject(std::shared_ptr<SceneObject> object)
{
    object->setId(++m_greatestId);
    m_objects.insert(std::make_pair(m_greatestId, object));
    return m_greatestId;
}

SceneObject* Scene::getObject(std::function<bool(SceneObject*)> predicate)
{
    return std::find_if(begin(), end(), [predicate](auto pr)->bool { return predicate(pr.second.get()); })->second.get();
}
SceneObject* Scene::getObject(std::string name)
{
    return getObject([name](SceneObject* object)->bool { return object->getName() == name; });
}
SceneObject* Scene::getObject(long long id)
{
    auto it = m_objects.find(id);
    if(it != m_objects.end())
        return it->second.get();
    return nullptr;
}

void Scene::setViewForWidget(sf::RenderTarget& target)
{
    // fill the window fully by default
    if(m_size == sf::Vector2f())
    {
        m_size = sf::Vector2f(target.getSize());
        m_autoResizable = true;
    }

    Widget::setViewForWidget(target);
}

}
