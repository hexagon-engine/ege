/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "TexturedObject2D.h"

namespace EGE
{

void TexturedObject2D::updateGeometry()
{
    if(m_geometryChanged)
    {
        //DBG(SCENE_DEBUG, "TexturedObject2D::updateGeometry");
        ResourceManager* resManager = m_owner->getLoop()->getResourceManager().lock().get();
        ASSERT(resManager);
        sf::Texture* texture = resManager->getTexture(m_texture).get();
        ASSERT(texture);
        m_sprite.setTexture(*texture);
        if(m_textureRect == sf::FloatRect())
            m_textureRect = sf::FloatRect(0.f, 0.f, texture->getSize().x, texture->getSize().y);
        m_sprite.setTextureRect((sf::IntRect)m_textureRect);
        m_sprite.setPosition(getPosition());
        if(m_centered)
            m_origin = m_textureRect.getSize() / 2.f;
        m_sprite.setOrigin(m_origin);
        m_sprite.setRotation(m_rotation);
        m_sprite.setScale(m_scale);

        m_geometryChanged = false;
    }
}

sf::FloatRect TexturedObject2D::getBoundingBox() const
{
    sf::Vector2f origin = m_origin;
    if(m_centered)
        origin = m_textureRect.getSize() / 2.f;

    sf::FloatRect rect(getPosition(), m_textureRect.getSize());
    rect.width *= m_scale.x;
    rect.height *= m_scale.y;
    rect.left -= origin.x * m_scale.x;
    rect.top -= origin.y * m_scale.y;
    return rect;
}

void TexturedObject2D::onUpdate(long long tickCounter)
{
    // TODO: it should be in render phase (it's not necessary on server)
    // maybe void renderUpdate() ??
    SceneObject2D::onUpdate(tickCounter);

    // Update geometry (only on client)
    if(m_owner->getLoop())
        updateGeometry();
}

void TexturedObject2D::render(sf::RenderTarget& target) const
{
    SceneObject2D::render(target);
    //DUMP(SCENE_DEBUG, m_sprite.getPosition().x);
    //DUMP(SCENE_DEBUG, m_sprite.getPosition().y);
    target.draw(m_sprite);
}

}
