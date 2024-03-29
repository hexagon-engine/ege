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

#include "TexturedPart.h"

#include "../Scene.h"
#include "../SceneObject.h"

namespace EGE
{

void TexturedPart::updateGeometry(Renderer&)
{
    if(!m_texture)
    {
        auto texture = getObject().getOwner().getLoop()->getResourceManager()->getTexture(m_textureName);
        ASSERT(texture);
        m_texture = &texture->getTexture();
    }
}

void TexturedPart::render(Renderer& renderer) const
{
    renderer.renderTexturedRectangle(position.x, position.y, m_texture->getSize().x, m_texture->getSize().y, *m_texture);
}

void TexturedPart::setTextureName(String tex)
{
    m_textureName = tex;
    setGeometryNeedUpdate();
}

bool TexturedPart::deserialize(SharedPtr<ObjectMap> data)
{
    if(!Part::deserialize(data))
        return false;
    position = Serializers::toVector2(data->get("pos"));
    m_textureName = data->getObject("textureName").asString().valueOr("");
    setGeometryNeedUpdate();
    return true;
}

}
