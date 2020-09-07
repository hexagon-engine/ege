/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/gui/Animatable.h>
#include <ege/util/Serializable.h>

namespace EGE
{

class Scene;

class SceneObject : public Animatable, public Serializable
{
public:
    SceneObject(Scene* owner, std::string name)
    : m_owner(owner), m_name(name) {}

    virtual void onUpdate(long long tickCounter);
    virtual void render(sf::RenderTarget& target) const = 0;

    bool isDead() const
    {
        return m_dead;
    }
    void setId(long long id)
    {
        m_id = id;
    }
    std::string getName() const
    {
        return m_name;
    }
    long long getId() const
    {
        return m_id;
    }

    virtual std::shared_ptr<ObjectMap> serialize();
    virtual void deserialize(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeMain();
    virtual void deserializeMain(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeExtended();
    virtual void deserializeExtended(std::shared_ptr<ObjectMap>);

protected:
    Scene* m_owner;
    bool m_dead = false;
    long long m_id = 0;
    std::string m_name;
};

}
