/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/gui/Animatable.h>

namespace EGE
{

class Scene;

class SceneObject : public Animatable
{
public:
    SceneObject(EGE::Scene* owner, std::string name)
    : m_owner(owner), m_name(name) {}

    virtual void onUpdate(long long tickCounter);
    virtual void render(sf::RenderTarget& target) = 0;

    bool isDead()
    {
        return m_dead;
    }
    void setId(long long id)
    {
        m_id = id;
    }
    std::string getName()
    {
        return m_name;
    }
    long long getId()
    {
        return m_id;
    }

protected:
    EGE::Scene* m_owner;
    bool m_dead = false;
    long long m_id = 0;
    std::string m_name;
};

}
