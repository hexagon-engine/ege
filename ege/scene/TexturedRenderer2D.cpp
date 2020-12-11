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
    //DBG(SCENE_DEBUG, "TexturedObject2D::updateGeometry");
    // Update texture
    ResourceManager* resManager = object.getOwner()->getLoop()->getResourceManager().get();
    ASSERT(resManager);
    m_texture = resManager->getTexture(m_textureName).get();
    ASSERT(m_texture);

    // Update texture rect
    if(m_textureRect == sf::FloatRect())
        m_textureRect = sf::FloatRect(0.f, 0.f, m_texture->getSize().x, m_texture->getSize().y);
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

void TexturedRenderer2D::render(const SceneObject& sceneObject, sf::RenderTarget& target, const RenderStates& states) const
{
    sf::Sprite sprite;
    SceneObject2D& so2d = (SceneObject2D&)sceneObject;
    sprite.setTexture(*m_texture);
    sprite.setTextureRect((sf::IntRect)m_textureRect);
    sprite.setPosition(so2d.getPosition());
    if(m_centered)
        so2d.setOrigin(m_textureRect.getSize() / 2.f);
    sprite.setOrigin(so2d.getOrigin());
    sprite.setRotation(so2d.getRotation());
    sprite.setScale(so2d.getScale());

    target.draw(sprite, states.sfStates());
}

}
