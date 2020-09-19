/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/gpo/GameplayObject.h>
#include <ege/gui/Animatable.h>
#include <ege/util/Serializable.h>

namespace EGE
{

class Scene;

class SceneObject : public Animatable, public GameplayObject
{
public:
    SceneObject(Scene* owner, std::string typeId)
    : GameplayObject(typeId), m_owner(owner) {}

    virtual void onUpdate(long long tickCounter);
    virtual void render(sf::RenderTarget& target) const = 0;
    virtual void preRenderUpdate() {}

    bool isDead() const
    {
        return m_dead;
    }
    void setObjectId(long long id)
    {
        if(!m_id)
            m_id = id;
    }
    std::string getName() const
    {
        return m_name;
    }
    void setName(std::string name)
    {
        m_name = name;
    }
    long long getObjectId() const
    {
        return m_id;
    }

    virtual std::shared_ptr<ObjectMap> serialize();
    virtual void deserialize(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeMain();
    virtual void deserializeMain(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeExtended();
    virtual void deserializeExtended(std::shared_ptr<ObjectMap>);

    // Clears main and extended changed flags. Called by server.
    // FIXME: it should be only callable by Server.

    void clearMainChangedFlag()
    {
        m_mainChanged = false;
    }

    void clearExtendedChangedFlag()
    {
        m_extendedChanged = false;
    }

    bool getMainChangedFlag() const
    {
         return m_mainChanged;
    }

    bool getExtendedChangedFlag() const
    {
         return m_extendedChanged;
    }

    void setDead()
    {
        m_dead = true;
    }

    Scene* getOwner()
    {
        return m_owner;
    }

protected:
    void setMainChanged(bool flag = true)
    {
        m_mainChanged = flag;
    }
    void setExtendedChanged(bool flag = true)
    {
        m_extendedChanged = flag;
    }

    Scene* m_owner;
    bool m_dead = false;
    long long m_id = 0;
    std::string m_name;
    bool m_mainChanged = true;
    bool m_extendedChanged = true;
};

}
