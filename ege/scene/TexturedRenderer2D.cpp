/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "TexturedRenderer2D.h"

#include "Scene.h"

namespace EGE
{

void TexturedRenderer2D::updateGeometry(Renderer&)
{
    //DBG(SCENE_DEBUG, "TexturedObject2D::updateGeometry");
    // Update texture
    ResourceManager* resManager = m_sceneObject.getOwner().getLoop()->getResourceManager().get();
    ASSERT(resManager);
    m_texture = resManager->getTexture(m_textureName).get();
    ASSERT(m_texture);

    // Update texture rect
    if(m_textureRect == sf::FloatRect())
        m_textureRect = sf::FloatRect(0.f, 0.f, m_texture->getTexture().getSize().x, m_texture->getTexture().getSize().y);
}

sf::FloatRect TexturedRenderer2D::getBoundingBox() const
{
    auto& sceneObject = (SceneObject2D&)m_sceneObject;

    Vec2d origin = sceneObject.getOrigin();
    if(m_centered)
    {
        auto _origin = m_textureRect.getSize() / 2.f;
        origin = {_origin.x, _origin.y};
    }

    auto pos = sceneObject.getPosition();
    auto size = m_textureRect.getSize();
    sf::FloatRect rect(pos.x, pos.y, size.x, size.y);
    rect.width *= sceneObject.getScale().x;
    rect.height *= sceneObject.getScale().y;
    rect.left -= origin.x * sceneObject.getScale().x;
    rect.top -= origin.y * sceneObject.getScale().y;
    return rect;
}

void TexturedRenderer2D::render(Renderer& renderer) const
{
    sf::Sprite sprite;
    SceneObject2D& so2d = (SceneObject2D&)m_sceneObject;
    sprite.setTexture(m_texture->getTexture());
    sprite.setTextureRect((sf::IntRect)m_textureRect);
    sprite.setPosition(so2d.getPosition().x, so2d.getPosition().y);
    if(m_centered)
    {
        auto origin = m_textureRect.getSize() / 2.f;
        so2d.setOrigin({origin.x, origin.y});
    }
    sprite.setOrigin(so2d.getOrigin().x, so2d.getOrigin().y);
    sprite.setRotation(so2d.getRotation());
    sprite.setScale(so2d.getScale().x, so2d.getScale().y);

    renderer.getTarget().draw(sprite, renderer.getStates().sfStates());
}

}
