/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ObjectRenderer.h"
#include "SceneObject2D.h"

#include <ege/gfx/RenderStates.h>
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
        m_textureName = name;
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
    }

    virtual void render(const SceneObject& object, sf::RenderTarget& target, const RenderStates& states) const;

private:
    virtual void updateGeometry(SceneObject& object);

    std::string m_textureName;
    sf::FloatRect m_textureRect;
    EGE::Texture* m_texture = nullptr;
    bool m_centered = false;
};

}
