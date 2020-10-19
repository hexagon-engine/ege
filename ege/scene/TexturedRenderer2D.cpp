/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "TexturedRenderer2D.h"

#include "Scene.h"

namespace EGE
{

void TexturedRenderer2D::updateGeometry(SceneObject& object)
{
    SceneObject2D& sceneObject = (SceneObject2D&)object;

    //DBG(SCENE_DEBUG, "TexturedObject2D::updateGeometry");
    ResourceManager* resManager = object.getOwner()->getLoop()->getResourceManager().lock().get();
    ASSERT(resManager);
    sf::Texture* texture = resManager->getTexture(m_texture).get();
    ASSERT(texture);
    m_sprite.setTexture(*texture);
    if(m_textureRect == sf::FloatRect())
        m_textureRect = sf::FloatRect(0.f, 0.f, texture->getSize().x, texture->getSize().y);
    m_sprite.setTextureRect((sf::IntRect)m_textureRect);
    m_sprite.setPosition(sceneObject.getPosition());
    if(m_centered)
        sceneObject.setOrigin(m_textureRect.getSize() / 2.f);
    m_sprite.setOrigin(sceneObject.getOrigin());
    m_sprite.setRotation(sceneObject.getRotation());
    m_sprite.setScale(sceneObject.getScale());
}

sf::FloatRect TexturedRenderer2D::getBoundingBox(const SceneObject& object) const
{
    SceneObject2D& sceneObject = (SceneObject2D&)object;

    sf::Vector2f origin = sceneObject.getOrigin();
    if(m_centered)
        origin = m_textureRect.getSize() / 2.f;

    sf::FloatRect rect(sceneObject.getPosition(), m_textureRect.getSize());
    rect.width *= sceneObject.getScale().x;
    rect.height *= sceneObject.getScale().y;
    rect.left -= origin.x * sceneObject.getScale().x;
    rect.top -= origin.y * sceneObject.getScale().y;
    return rect;
}

void TexturedRenderer2D::render(const SceneObject&, sf::RenderTarget& target, const RenderStates& states) const
{
    target.draw(m_sprite, states.sfStates());
}

}
