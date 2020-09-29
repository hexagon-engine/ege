/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ObjectRenderer.h"
#include "SceneObject2D.h"

#include <ege/gui/GUIGameLoop.h>
#include <ege/resources/ResourceManager.h>

namespace EGE
{

class TexturedRenderer2D : public ObjectRenderer
{
public:
    TexturedRenderer2D(std::shared_ptr<Scene> scene)
    : ObjectRenderer(scene) {}

    void setTextureName(std::string name)
    {
        m_texture = name;
        setGeometryNeedUpdate();
    }
    void setTextureRect(sf::FloatRect rect)
    {
        m_textureRect = rect;
        setGeometryNeedUpdate();
    }

    virtual sf::FloatRect getBoundingBox(const SceneObject& object) const;

    void center(bool centered = true)
    {
        m_centered = centered;
        setGeometryNeedUpdate();
    }

    virtual void render(const SceneObject& object, sf::RenderTarget& target) const;

private:
    virtual void updateGeometry(SceneObject& object);

    std::string m_texture;
    sf::FloatRect m_textureRect;
    sf::Sprite m_sprite;
    bool m_centered = false;
};

}
