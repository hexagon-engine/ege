/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
        log(LogLevel::Warning) << "Failed to add texture " + tex->getName() + " to"
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
