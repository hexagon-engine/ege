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

#include "ResourceManager.h"

#include "SFMLTexture.h"

#include <ege/debug/Logger.h>
#include <sys/stat.h>

namespace EGE
{

void ResourceManager::clear()
{
    // Reset ResourceManager like the constructor
    m_loadedFonts.clear();
    m_loadedTextures.clear();
    m_loadedCursors.clear();
    m_unknownTexture = nullptr;
    m_resourcePath = "res";
    m_defaultFont = "";
}

std::shared_ptr<Texture> ResourceManager::loadTextureFromFile(std::string fileName)
{
    std::shared_ptr<SFMLTexture> texture = make<SFMLTexture>();
    if(!texture->loadFromFile(m_resourcePath + "/" + fileName))
    {
        err(LogLevel::Error) << "0005 EGE/resources: could not load resource: TEXTURE " << fileName;
        m_error = true;
        return nullptr;
    }
    addTexture(fileName, texture);
    return texture;
}

std::shared_ptr<sf::Font> ResourceManager::loadFontFromFile(std::string fileName)
{
    std::shared_ptr<sf::Font> font = make<sf::Font>();
    if(!font->loadFromFile(m_resourcePath + "/" + fileName))
    {
        err(LogLevel::Error) << "0006 EGE/resources: could not load resource: FONT " << fileName;
        m_error = true;
        return nullptr;
    }
    addFont(fileName, font);
    return font;
}

std::shared_ptr<sf::Shader> ResourceManager::loadShaderFromFile(std::string fileName, sf::Shader::Type type)
{
    std::shared_ptr<sf::Shader> shader(new sf::Shader);
    if(!shader->loadFromFile(m_resourcePath + "/" + fileName, type))
    {
        err(LogLevel::Error) << "EGE/resources: could not load resource: [" << (int)type << "] SHADER " << fileName;
        m_error = true;
        return nullptr;
    }
    addShader(fileName, shader);
    return shader;
}

std::shared_ptr<sf::Shader> ResourceManager::loadShaderFromFile(std::string name, std::string vertexShader, std::string fragmentShader)
{
    std::shared_ptr<sf::Shader> shader(new sf::Shader);
    if(!shader->loadFromFile(m_resourcePath + "/" + vertexShader, m_resourcePath + "/" + fragmentShader))
    {
        err(LogLevel::Error) << "EGE/resources: could not load resource: [VF] SHADER " << vertexShader << ", " << fragmentShader;
        m_error = true;
        return nullptr;
    }
    addShader(name, shader);
    return shader;
}

std::shared_ptr<sf::Shader> ResourceManager::loadShaderFromFile(std::string name, std::string vertexShader, std::string geometryShader, std::string fragmentShader)
{
    std::shared_ptr<sf::Shader> shader(new sf::Shader);
    if(!shader->loadFromFile(m_resourcePath + "/" + vertexShader, m_resourcePath + "/" + geometryShader, m_resourcePath + "/" + fragmentShader))
    {
        err(LogLevel::Error) << "EGE/resources: could not load resource: [VGF] SHADER " << vertexShader << ", " << geometryShader << ", " << fragmentShader;
        m_error = true;
        return nullptr;
    }
    addShader(name, shader);
    return shader;
}

std::shared_ptr<sf::Cursor> ResourceManager::loadCursorFromFile(std::string)
{
    // TODO
    return nullptr;
}

void ResourceManager::addTexture(std::string name, std::shared_ptr<Texture> texture)
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

void ResourceManager::addShader(std::string name, std::shared_ptr<sf::Shader> shader)
{
    auto it = m_loadedShaders.find(name);
    if(it == m_loadedShaders.end())
    {
        m_loadedShaders.insert(std::make_pair(name, shader));
    }
    else if(shader != nullptr)
    {
        it->second = shader;
    }
}

std::shared_ptr<Texture> ResourceManager::getTexture(std::string name)
{
    auto it = m_loadedTextures.find(name);
    if(it == m_loadedTextures.end())
    {
        err(LogLevel::Error) << "0008 EGE/resources: invalid TEXTURE requested: " << name << ", falling back to unknown texture";
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
        err(LogLevel::Error) << "0009 EGE/resources: invalid FONT requested: " << name;
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
        err(LogLevel::Error) << "0023 EGE/resources: invalid CURSOR requested: " << name;
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
        err(LogLevel::Error) << "0025 EGE/resources: invalid SYSTEM CURSOR requested: " << (int)type;
        if(!m_systemCursorError)
            return loadSystemCursor(type);
        else
            return nullptr;
    }
    return cursor;
}

std::shared_ptr<sf::Shader> ResourceManager::getShader(std::string name)
{
    auto it = m_loadedShaders.find(name);
    if(it == m_loadedShaders.end())
    {
        err(LogLevel::Error) << "0008 EGE/resources: invalid SHADER requested: " << name;
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<sf::Cursor> ResourceManager::loadSystemCursor(sf::Cursor::Type type)
{
    std::shared_ptr<sf::Cursor> cursor(new sf::Cursor);
    if(!cursor->loadFromSystem(type))
    {
        err(LogLevel::Error) << "0026 EGE/resources: could not load resource: SYSTEM CURSOR" << (int)type;
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
            err() << "000B EGE/resources: invalid FONT requested to be default: " << name;
            return false;
        }
        m_defaultFont = name;
        return true;
    }
}

void ResourceManager::setUnknownTexture(std::shared_ptr<Texture> texture)
{
    m_unknownTexture = texture;
}

bool ResourceManager::setResourcePath(std::string path)
{
    struct stat _s;
    if(stat(path.c_str(), &_s) < 0)
    {
        err() << "0007 EGE/resources: could not open resource directory: " << path;
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
