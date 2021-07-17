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

#include "AtlasTexture.h"

#include <ege/debug/Logger.h>

namespace EGE
{

AtlasTexture::TextureRect AtlasTexture::registerIcon(SharedPtr<SFMLTexture> tex)
{
    // Check if texture size is equal to unit size.
    auto texSize = tex->getTexture().getSize();

    if(!tex)
        return {};

    if(texSize.x != m_unit.x || texSize.y != m_unit.y)
    {
        ege_log.warning() << "AtlasTexture: Failed to add texture " + tex->getName() + " to"
                                    "atlas, the texture size must be equal to unit size";
        return {};
    }

    // Check if new icon will fit the atlas.
    if(!m_texture)
        return {};

    if(m_atlasSize * m_atlasSize < m_textureMap.size())
        return {};

    // Add texture to texture map
    unsigned x = m_textureMap.size() % m_atlasSize;
    unsigned y = m_textureMap.size() / m_atlasSize;

    TextureRect rect;
    rect.position = {x, y};
    rect.size = m_unit;
    m_textureMap.insert(std::make_pair(tex->getName(), rect));

    // Copy texture to atlas
    m_texture->getTexture().update(tex->getTexture(), x * m_unit.x, y * m_unit.y);
    return rect;
}

Optional<AtlasTexture::TextureRect> AtlasTexture::getIcon(String name)
{
    auto it = m_textureMap.find(name);
    if(it != m_textureMap.end())
        return {it->second};
    return {};
}

void AtlasTexture::allocateAtlas(unsigned unitCount)
{
    m_texture = std::make_unique<SFMLTexture>();
    m_texture->getTexture().create(unitCount * m_unit.x, unitCount * m_unit.y);
    m_atlasSize = unitCount;
}

}
