/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>

namespace EGE
{

class ResourceManager
{
public:
    virtual bool reload() = 0;
    void clear();
    bool isError();

protected:

    // Loads specified resources from file.
    // Returns nullptr when texture couldn't be loaded.
    std::shared_ptr<sf::Texture> loadTextureFromFile(std::string fileName);
    std::shared_ptr<sf::Font> loadFontFromFile(std::string fileName);

    // Adds preloaded resources to ResourceManager.
    // Useful when you want to add your options to Texture before adding to RM.
    void addTexture(std::string name, std::shared_ptr<sf::Texture> texture);
    void addFont(std::string name, std::shared_ptr<sf::Font> font);

    // TODO
    bool loadSystemFont(std::string fileName) { ASSERT(false); return false; }

    std::shared_ptr<sf::Texture> getTexture(std::string name);
    std::shared_ptr<sf::Font> getFont(std::string name);

    // Replaces default unknown texture with your own.
    void setUnknownTexture(std::shared_ptr<sf::Texture> texture = nullptr);

    // Sets the resource path (absolute or relative to current working directory)
    bool setResourcePath(std::string path);

private:
    std::shared_ptr<sf::Texture> m_unknownTexture;

    std::map<std::string, std::shared_ptr<sf::Texture>> m_loadedTextures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_loadedFonts;

    bool m_error = false;
    std::string m_resourcePath = "res";
};

}
