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
    // Reset ResourceManager like the constructor
    m_loadedFonts.clear();
    m_loadedTextures.clear();
    m_unknownTexture = nullptr;
    m_resourcePath = "res";
    m_defaultFont = "";
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

std::shared_ptr<sf::Cursor> ResourceManager::loadCursorFromFile(std::string)
{
    // TODO
    return nullptr;
}

void ResourceManager::addTexture(std::string name, std::shared_ptr<sf::Texture> texture)
{
    auto it = m_loadedTextures.find(name);
    if(it == m_loadedTextures.end())
    {
        m_loadedTextures.insert(std::make_pair(name, texture));
    }
    else if(texture != nullptr)
    {
        it->second = texture;
    }
}

void ResourceManager::addFont(std::string name, std::shared_ptr<sf::Font> font)
{
    auto it = m_loadedFonts.find(name);
    if(it == m_loadedFonts.end())
    {
        m_loadedFonts.insert(std::make_pair(name, font));
    }
    else if(font != nullptr)
    {
        it->second = font;
    }
}

void ResourceManager::addCursor(std::string name, std::shared_ptr<sf::Cursor> cursor)
{
    auto it = m_loadedCursors.find(name);
    if(it == m_loadedCursors.end())
    {
        m_loadedCursors.insert(std::make_pair(name, cursor));
    }
    else if(cursor != nullptr)
    {
        it->second = cursor;
    }
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
    if(!it->second)
    {
        // TODO: call some user handler to allow him
        // to change texture settings after loading
        return loadTextureFromFile(name);
    }
    return it->second;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(std::string name)
{
    auto it = m_loadedFonts.find(name);
    if(it == m_loadedFonts.end())
    {
        std::cerr << "0009 EGE/resources: invalid FONT requested: " << name << std::endl;
        if(name != m_defaultFont && !m_defaultFont.empty())
        {
            auto font = getDefaultFont();
            if(!font)
                return nullptr;
            m_loadedFonts[name] = font;
            return font;
        }
        return nullptr;
    }
    if(!it->second)
    {
        // TODO: call some user handler to allow him
        // to change texture settings after loading
        return loadFontFromFile(name);
    }
    return it->second;
}

std::shared_ptr<sf::Cursor> ResourceManager::getCursor(std::string name)
{
    auto it = m_loadedCursors.find(name);
    if(it == m_loadedCursors.end())
    {
        std::cerr << "0023 EGE/resources: invalid CURSOR requested: " << name << std::endl;
        return nullptr;
    }
    if(!it->second)
    {
        // TODO: call some user handler to allow him
        // to change texture settings after loading
        return loadCursorFromFile(name);
    }
    return it->second;
}

std::shared_ptr<sf::Cursor> ResourceManager::getCursor(sf::Cursor::Type type)
{
    auto cursor = getCursor("/EGE::System::/ /_" + std::to_string((int)type));
    if(!cursor)
    {
        std::cerr << "0025 EGE/resources: invalid SYSTEM CURSOR requested: " << (int)type << std::endl;
        if(!m_systemCursorError)
            return loadSystemCursor(type);
        else
            return nullptr;
    }
    return cursor;
}

std::shared_ptr<sf::Cursor> ResourceManager::loadSystemCursor(sf::Cursor::Type type)
{
    std::shared_ptr<sf::Cursor> cursor(new sf::Cursor);
    if(!cursor->loadFromSystem(type))
    {
        std::cerr << "0026 EGE/resources: could not load resource: SYSTEM CURSOR" << (int)type << std::endl;
        m_error = true;
        m_systemCursorError = true;
        return nullptr;
    }
    addCursor("/EGE::System::/ /_" + std::to_string((int)type), cursor);
    return cursor;
}

std::shared_ptr<sf::Font> ResourceManager::getDefaultFont()
{
    return getFont(m_defaultFont);
}
bool ResourceManager::setDefaultFont(std::string name)
{
    auto font = getFont(name);
    if((font && font != getDefaultFont()) || name.empty())
    {
        m_defaultFont = name;
        return true;
    }
    else
    {
        if(!loadFontFromFile(name))
        {
            std::cerr << "000B EGE/resources: invalid FONT requested to be default: " << name << std::endl;
            return false;
        }
        m_defaultFont = name;
        return true;
    }
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
