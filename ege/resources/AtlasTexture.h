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
