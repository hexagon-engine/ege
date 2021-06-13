/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
    auto pos = m_sceneObject.getPosition();
    auto size = m_textureRect.getSize();
    sf::FloatRect rect(pos.x, pos.y, size.x, size.y);
    return rect;
}

void TexturedRenderer2D::render(Renderer& renderer) const
{
    sf::Sprite sprite;
    sprite.setTexture(m_texture->getTexture());
    sprite.setTextureRect((sf::IntRect)m_textureRect);
    sprite.setPosition(m_sceneObject.getPosition().x, m_sceneObject.getPosition().y);
    sprite.setRotation(m_sceneObject.getRotation());

    renderer.getTarget().draw(sprite, renderer.getStates().sfStates());
}

}
