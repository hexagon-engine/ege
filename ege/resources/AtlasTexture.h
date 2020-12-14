/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SFMLTexture.h"
#include "Texture.h"

#include <ege/util/Optional.h>

namespace EGE
{

class AtlasTexture : public Texture
{
public:
    struct TextureRect
    {
        Vec2u position;
        Vec2u size;
    };

    // Add an icon to atlas.
    // Returns: atlas texture rect.
    // For now, the texture size must be unit.
    virtual TextureRect registerIcon(SharedPtr<SFMLTexture> tex);

    // Get coordinates of specified icon in atlas.
    virtual Optional<TextureRect> getIcon(String name);

    virtual sf::Texture& getTexture() { return m_texture->getTexture(); }

    // Set size, which multiple are atlas coordinates.
    // The default unit is 16x16.
    void setUnit(Vec2u unit) { m_unit = unit; }

    // (Re)Allocates atlas texture. It's required for now.
    void allocateAtlas(unsigned unitCount);

private:
    Map<String, TextureRect> m_textureMap;
    UniquePtr<SFMLTexture> m_texture;
    Vec2u m_unit {16, 16};
    unsigned m_atlasSize = 0;
};

}
