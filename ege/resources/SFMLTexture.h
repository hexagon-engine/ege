/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Texture.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

class SFMLTexture : public Texture
{
public:
    static SharedPtr<SFMLTexture> fromFile(const EGE::String& fileName)
    {
        auto tex = make<SFMLTexture>();
        if(!tex->loadFromFile(fileName))
            return {};
        tex->setName(fileName);
        return tex;
    }

    virtual bool loadFromFile(const EGE::String& fileName)
    {
        setName(fileName);
        return m_texture.loadFromFile(fileName);
    }

    virtual sf::Texture& getTexture() { return m_texture; }

private:
    sf::Texture m_texture;
};

}
