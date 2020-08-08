/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ResourceManager.h"

#include <sys/stat.h>

namespace EGE
{

void ResourceManager::clear()
{
    m_loadedFonts.clear();
    m_loadedTextures.clear();
}

std::shared_ptr<sf::Texture> ResourceManager::loadTextureFromFile(std::string fileName)
{
    std::shared_ptr<sf::Texture> texture(new sf::Texture);
    if(!texture->loadFromFile(m_resourcePath + "/" + fileName))
    {
        std::cerr << "0005 EGE/resources: could not load resource: TEXTURE " << fileName << std::endl;
        m_error = true;
        return nullptr;
    }
    addTexture(fileName, texture);
    return texture;
}

std::shared_ptr<sf::Font> ResourceManager::loadFontFromFile(std::string fileName)
{
    std::shared_ptr<sf::Font> font(new sf::Font);
    if(!font->loadFromFile(m_resourcePath + "/" + fileName))
    {
        std::cerr << "0006 EGE/resources: could not load resource: FONT " << fileName << std::endl;
        m_error = true;
        return nullptr;
    }
    addFont(fileName, font);
    return font;
}

void ResourceManager::addTexture(std::string name, std::shared_ptr<sf::Texture> texture)
{
    ASSERT(texture);
    ASSERT(texture->getSize().x > 0 && texture->getSize().y > 0);
    m_loadedTextures.insert(std::make_pair(name, texture));
}

void ResourceManager::addFont(std::string name, std::shared_ptr<sf::Font> font)
{
    ASSERT(font);
    ASSERT(!font->getInfo().family.empty());
    m_loadedFonts.insert(std::make_pair(name, font));
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(std::string name)
{
    auto it = m_loadedTextures.find(name);
    if(it == m_loadedTextures.end())
    {
        std::cerr << "0008 EGE/resources: invalid TEXTURE requested: " << name << ", falling back to unknown texture" << std::endl;
        m_loadedTextures[name] = m_unknownTexture;
        return m_unknownTexture;
    }
    return it->second;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(std::string name)
{
    auto it = m_loadedFonts.find(name);
    if(it == m_loadedFonts.end())
    {
        std::cerr << "0009 EGE/resources: invalid FONT requested: " << name << std::endl;
        m_loadedFonts[name] = nullptr;
        return nullptr;
    }
    return it->second;
}

void ResourceManager::setUnknownTexture(std::shared_ptr<sf::Texture> texture)
{
    m_unknownTexture = texture;
}

bool ResourceManager::setResourcePath(std::string path)
{
    struct stat _s;
    if(stat(path.c_str(), &_s) < 0)
    {
        std::cerr << "0007 EGE/resources: could not open resource directory: " << path << std::endl;
        return false;
    }
    m_resourcePath = path;
    return true;
}

bool ResourceManager::isError()
{
    return m_error;
}

}
