/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Scene.h"
#include "SceneObject2D.h"

#include <ege/gui/GUIGameLoop.h>
#include <ege/resources/ResourceManager.h>

namespace EGE
{

class TexturedObject2D : public SceneObject2D
{
public:
    TexturedObject2D(Scene* owner, std::string name)
    : SceneObject2D(owner, name) {}

    void setTextureName(std::string name)
    {
        m_texture = name;
        m_geometryChanged = true;
    }
    void setTextureRect(sf::FloatRect rect)
    {
        m_textureRect = rect;
        m_geometryChanged = true;
    }

    sf::FloatRect getBoundingBox() const
    {
        sf::Vector2f origin = m_origin;
        if(m_centered)
            origin = m_textureRect.getSize() / 2.f;

        sf::FloatRect rect(getPosition() - origin, m_textureRect.getSize());
        return rect;
    }

    void setPosition(sf::Vector2f position)
    {
        SceneObject2D::setPosition(position);
        m_geometryChanged = true;
    }

    void center(bool centered = true)
    {
        m_centered = centered;
        m_geometryChanged = true;
    }

    void onUpdate(long long tickCounter);

    virtual void render(sf::RenderTarget& target) const;

private:
    void updateGeometry();

    std::string m_texture;
    sf::FloatRect m_textureRect;
    sf::Sprite m_sprite;
    bool m_geometryChanged = true;
    bool m_centered = false;
};

}
